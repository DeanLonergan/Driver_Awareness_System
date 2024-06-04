/*@!Encoding:1252*/
includes{
#include ".\DataSources_Templates\ASNV_Template_BaseDatatypes.cin"
#include ".\DataSources_Templates\ASNV_Template_CAM.cin"
#include ".\DataSources_Templates\ASNV_Template_DENM.cin"
#include ".\DataSources_Templates\ASNV_Template_MAP.cin"
#include ".\DataSources_Templates\ASNV_Template_SPAT.cin"
#include ".\DataSources_Templates\ASNV_Template_IVIM.cin"
#include ".\Nodes\Position.cin"
}

variables
{
    int quadrant;
    int adjustedQuadrant;
    double vehicleCamLong = 0;
    double vehicleCamLat = 0;
    double distanceToCam;
    double DUTLong = 0;
    double DUTLat = 0;
    double CarLong = 0;
    double CarLat = 0;
    double deltaLong = 0;
    double deltaLat = 0;
    double absDeltaLat = 0;
    double absDeltaLong = 0;
    long DUTHeading = 0; // Heading of DUT
    long CarHeading = 0; // Heading of Car

    long v_r;        // rear vehicle velocity in m/s
    long v_f;        // front vehicle velocity in m/s
    float t;         // reasonable response time in s, changed from ?
    float max_accel; // maximum rate of acceleration in m/s^2
    float max_brake; // maximum rate of braking in m/s^2
    float min_brake; // minimum rate of braking in m/s^2
    long d_min;      // safe longitudinal distance
    long d_current;  // current longitudinal distance
}

on start
{
    enum CallbackType
    {
        OnRx,
        PreTx
    };
    C2xRegisterCallback(OnRx, "OnRxCAM", "CAM");
    C2xRegisterCallback(PreTx, "OnPreTxCAM", "CAM");

    t = 1.3;         // reasonable response time, in seconds, as per IEEE 2846
    max_accel = 2.9; // maximum rate of acceleration, in m/s^2, as per IEEE 2846
    max_brake = 4.4; // maximum rate of braking, in m/s^2, as per IEEE 2846
    min_brake = 3.0; // minimum rate of braking, in m/s^2, as per IEEE 2846
    
    @sysvar::carFrontClose = 0;
    @sysvar::carFrontFar = 0;
    @sysvar::carRight = 0;
    @sysvar::carLeft = 0;
    @sysvar::carRear = 0;
}

void evaluateRelativePosition(long channel, long dir, long radioChannel, long signalStrength, long signalQuality, long packet)
{
    deltaLat = CarLat - DUTLat;
    deltaLong = CarLong - DUTLong;

    // Implementing absolute value for double manually
    absDeltaLat = deltaLat > 0 ? deltaLat : -deltaLat;
    absDeltaLong = deltaLong > 0 ? deltaLong : -deltaLong;

    @sysvar::carFrontFar = 0;
    @sysvar::carFrontClose = 0;
    @sysvar::carRight = 0;
    @sysvar::carLeft = 0;
    @sysvar::carRear = 0;

    // Determine the more precise direction based on delta comparisons

    if (deltaLat >= 0 && deltaLong >= 0)
    {
        quadrant = 1; // Northeast
    }
    else if (deltaLat < 0 && deltaLong >= 0)
    {
        quadrant = 2; // Southeast
    }
    else if (deltaLat < 0 && deltaLong < 0)
    {
        quadrant = 3; // Southwest
    }
    else
    {
        quadrant = 4; // Northwest
    }

    // Adjust quadrant based on DUTHeading
    adjustedQuadrant = (quadrant + (DUTHeading / 90)) % 4;
    if (adjustedQuadrant == 0)
        adjustedQuadrant = 4;

    // Set sysvars based on adjusted quadrant
    switch (adjustedQuadrant)
    {
    case 1: // Front
        @sysvar::carFrontFar = 1;
        checkLongitudinalDistance(channel, dir, radioChannel, signalStrength, signalQuality, packet);
        write("Car detected in front of the DUT");
        break;
    case 2: // Right
        if (distanceToCam < 5)
        {
          write("Car detected to the right of the DUT");
          @sysvar::carRight = 1;
        }
        break;
    case 3: // Rear
        if (distanceToCam < 20)
        {
          write("Car detected to the left of the DUT");
          @sysvar::carRear = 1;
        }
        break;
    case 4: // Left
        if (distanceToCam < 5)
        {
          write("Car detected beind the DUT");
          @sysvar::carLeft = 1;
        }
        break;
    }
}

void OnRxCAM(long channel, long dir, long radioChannel, long signalStrength, long signalQuality, long packet)
{
    struct CAM camPdu;

    double camLong;
    double camLat;
    long camStationId;
    long camSpeed;
    long camHeading;

    if (!API_GetCAMParams(packet, camPdu))
    {
        camStationId = camPdu.header.stationID;
        camLong = camPdu.cam.camParameters.basicContainer.referencePosition.longitude / 1e07;
        camLat = camPdu.cam.camParameters.basicContainer.referencePosition.latitude / 1e07;
        camSpeed = camPdu.cam.camParameters.highFrequencyContainer.basicVehicleContainerHighFrequency.speed.speedValue;
        camHeading = camPdu.cam.camParameters.highFrequencyContainer.basicVehicleContainerHighFrequency.heading.headingValue;

        if (camStationId == 3)
        {
            DUTLat = camLat;
            DUTLong = camLong;
            DUTHeading = (camHeading / 10) % 360;
            @sysvar::dutHeading = camHeading / 10;
            @sysvar::dutSpeed = (camSpeed / 100) * 3.6;
        }
        else if (camStationId == 4)
        {
            CarLat = camLat;
            CarLong = camLong;
            CarHeading = camHeading / 10;
            evaluateRelativePosition(channel, dir, radioChannel, signalStrength, signalQuality, packet); // Evaluate relative position every time Car's position is updated
        }
    }
}

void OnPreTxCAM(LONG packet)
{
    struct CAM camPdu;
    // Save the current vehicle's co-ordinates for future distance calculations.
    if (!API_GetCAMParams(packet, camPdu))
    {
        vehicleCamLong = camPdu.cam.camParameters.basicContainer.referencePosition.longitude / 1e07;
        vehicleCamLat = camPdu.cam.camParameters.basicContainer.referencePosition.latitude / 1e07;
    }
}

// Function which calculates the safe longitudinal distance for a following car, based on RSS
long safeLongitudinalDistance(long v_r, long v_f, float t, float max_accel, float min_brake, float max_brake)
{
    long d_min;
    d_min = (v_r * t) + ((1 / 2) * (max_accel) * (t * t)) + 
            ((v_r + (t * max_accel)) * (v_r + (t * max_accel))) / 
            ((2 * min_brake)) - ((v_f * v_f) / (2 * max_brake));

    return d_min;
}

void checkLongitudinalDistance(long channel, long dir, long radioChannel, long signalStrength, long signalQuality, long packet)
{
    struct CAM camPdu;

    double camLong;
    double camLat;
    double v_fLong;
    double v_fLat;
    long camStationType;
    long camSpeed;
    long camHeading;
    long camStationId;

    if (!API_GetCAMParams(packet, camPdu))
    {
        camStationId = camPdu.header.stationID;
        camLong = camPdu.cam.camParameters.basicContainer.referencePosition.longitude / 1e07;
        camLat = camPdu.cam.camParameters.basicContainer.referencePosition.latitude / 1e07;
        camHeading = camPdu.cam.camParameters.highFrequencyContainer.basicVehicleContainerHighFrequency.heading.headingValue;
        camSpeed = camPdu.cam.camParameters.highFrequencyContainer.basicVehicleContainerHighFrequency.speed.speedValue;
        camStationType = camPdu.cam.camParameters.basicContainer.stationType;

        // Calculate the linear distance to the CAM station whose message was received.
        distanceToCam = API_PosCalcDistance(vehicleCamLat, vehicleCamLong, camLat, camLong);

        if (camStationId == 3)
        {
            v_r = camSpeed / 100;
        }
        if (camStationId == 4)
        {
            v_f = camSpeed / 100;
            d_current = distanceToCam;
            v_fLong = camLong;
            v_fLat = camLat;
        }

        if (d_current < 80)
        {
            @sysvar::carFrontFar = 1;
            @sysvar::carFrontClose = 0;
        }

        if (d_current < 15)
        {
            @sysvar::carFrontClose = 1;
        }

        if (d_current > 80)
        {
            @sysvar::carFrontFar = 0;
        }
        d_min = safeLongitudinalDistance(v_r, v_f, t, max_accel, min_brake, max_brake);
        if (d_current < d_min)
        { 
            @sysvar::carFrontClose = 1;
        }
    }
}