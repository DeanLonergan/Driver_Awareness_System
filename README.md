# Driver Awareness System
This was my Final Year Project that I worked on during my last year of BSc (Hons) in Applied Computing at S.E.T.U. I developed it using Vector CANoe, an industry-standard tool that I got to grips with during several of the automotive-specific modules that I attended as part of the Automotive and Automation stream.

My goal was to apply the principles of functional safety during the development of an automotive software component. The Driver Awareness System, or D.A.S., uses data from Car2X/V2X signals to enhance a driver's situational awareness and alert them to possible conflicts.

The D.A.S. achieves this using location and speed data gathered through the Car2X signals and known kinematic data derived from IEEE 2846. The D.A.S. processes this data and presents it to the driver in a non-abrasive and subtle way using cues on the dashboard, HUD, and through the interior ambient lighting.

I enjoyed working on this project and I am very excited to see what further developments will be made using Car2X signals within the automotive industry. Below is the poster I created for the Computing Project Expo at S.E.T.U. At this expo, my project was given the Highly Commended Award presented by Kargo, which was a very special moment for me and an excellent way to wrap up my time at S.E.T.U.

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/4839f709-fed0-4888-80d8-ccf53d68ddc0" alt="DAS poster">
</div>

## How it works
<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/8fc80fd3-b9e7-40ed-ad34-95391ec3696e" alt="Image 1">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/5440628c-c1d8-4978-80a5-764facb422f1" alt="Image 2">
</div>

* Use the longitude and latitude of both vehicles to determine which quadrant (NE, NW, SE, SW) Car 2 is in relative to the DUT. 
* Adjust the quadrants based on the DUT's heading to align with the DUT's orientation. 
* Use an equation developed by mobileye.com and both vehicles' speeds', to determine if the vehicle ahead is at a safe distance.
* Update system variables to indicate whether the other vehicle is in front, right, left, or rear of the DUT and at a safe distance.

*note: DUT = Device Under Test (the EGO vehicle)*

## Calculating Safe Longitudinal Distance

If there is a vehicle ahead:

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/5ee3b9a1-8c04-4f8a-a07c-4b829de4d311" alt="Calculating Distance 1">
</div>

Use the vehicle's distance, speed, and known kinematic data (derived from IEEE 2846) in the Safe Longitudinal Distance equation:

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/3965ab22-ca0d-4469-8960-35236817f40e" alt="Calculating Distance 2">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/82ab3282-2c13-458b-858b-e27b1ffcb699" alt="Calculating Distance 3">
</div>

If it is determined that the vehicle ahead is at an unsafe longitudinal distance, inform the driver accordingly:

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/54d2d2f7-1cdf-40cc-88ff-3ef73c527c6a" alt="Calculating Distance 4">
</div>

*notes:*  
*Car_Speed = The speed of the vehicle ahead*  
*DUT_Speed = The speed of the “Device under test”*  
*DUT_max_accel = The maximum rate of acceleration of the DUT*  
*DUT_max_brake = The maximum rate of deceleration of the DUT*  
*DUT_min_brake = The minimum, practical, rate of deceleration of the DUT*  
*DUT_react_time = The approximate reaction time of the driver of the DUT*  
*Wherein the vehicle kinematics (DUT_react_time, DUT_max_accel, etc.) have been defined by IEEE 2846*

*sources:*  
*https://www.mobileye.com/technology/responsibility-sensitive-safety/*  
*https://standards.ieee.org/*

## The Driver Awareness System in Action

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/dd7ac928-3135-41f7-a422-1ef0d0a695c8" alt="Safe Longitudinal Distance">
</div>


