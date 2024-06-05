# Driver Awareness System
This was my Final Year Project that I worked on during my last year of BSc (Hons) in Applied Computing at S.E.T.U. I developed it using Vector CANoe, an industry-standard tool that I got to grips with during several of the automotive-specific modules that I attended as part of the Automotive and Automation stream.

My goal was to apply the principles of functional safety during the development of an automotive software component. The Driver Awareness System, or D.A.S., uses data from Car2X/V2X signals to enhance a driver's situational awareness and alert them to possible conflicts.

The D.A.S. achieves this using location and speed data gathered through the Car2X signals and known kinematic data derived from IEEE 2846. The D.A.S. processes this data and presents it to the driver in a non-abrasive and subtle way using cues on the dashboard, HUD, and through the interior ambient lighting.

I enjoyed working on this project and I am very excited to see what further developments will be made using Car2X signals within the automotive industry. Below is the poster I created for the Computing Project Expo at S.E.T.U. At this expo, my project was given the Highly Commended Award presented by Kargo, which was a very special moment for me and an excellent way to wrap up my time at S.E.T.U.

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/4839f709-fed0-4888-80d8-ccf53d68ddc0" alt="DAS poster">
</div>

## The Driver Awareness System in action
In this example, simulated within CANoe, we can see the yellow car approach the DUT rapidly from behind, overtake it, then pull back in front at a now unsafe longitudinal distance. The D.A.S. monitors the vehicle throughout and presents the appropriate information to the driver through the dashboard and HUD displays, along with the ambient lighting on the driver's side door.

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/dd7ac928-3135-41f7-a422-1ef0d0a695c8" alt="Safe Longitudinal Distance">
</div>

*note: DUT = Device Under Test (the EGO vehicle)*

## How it works

### System Architecture

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/d7e66336-ac04-4907-a28d-11be3fc0acb7" alt="Message Bus Architecture">
</div>

### Locating the other vehicle

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/8fc80fd3-b9e7-40ed-ad34-95391ec3696e" alt="Image 1">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/5440628c-c1d8-4978-80a5-764facb422f1" alt="Image 2">
</div>

* Use the longitude and latitude of both vehicles to determine which quadrant (NE, NW, SE, SW) the other car is in, relative to the DUT.
* Adjust the quadrants based on the DUT's heading to align with the DUT's orientation.
* Use an equation developed by Mobileye.com and both vehicles' speeds to determine if the vehicle ahead is at a safe distance.
* Update system variables to indicate whether the other vehicle is in front, right, left, or rear of the DUT and if it is at a safe distance.

### Calculating Safe Longitudinal Distance
If there is a vehicle ahead:

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/5ee3b9a1-8c04-4f8a-a07c-4b829de4d311" alt="Calculating Distance 1">
</div>

Use the vehicles' speeds and known kinematic data (derived from IEEE 2846) in the Safe Longitudinal Distance equation:

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

## The Application of Functional Safety
Functional Safety, as defined by ISO 26262, ensures that automotive systems respond correctly to inputs to reduce risks from malfunctions. In this project, the implementation of functional safety involved rigorous safety analysis, hazard identification, and the development of safety measures to manage and mitigate risks associated with the Driver Awareness System.

This process led to the identification and classification of potential hazards, the formulation of safety goals, and the assignment of an Automotive Safety Integrity Level (ASIL) of B. Integrating ISO 26262 in this project changed the development approach, requiring detailed documentation, systematic validation, and verification at each stage. 

The benefits of this approach include:
* Enhanced reliability
* Reduced risk of system failures
* Improved overall safety and operational effectiveness of the D.A.S. in real-time vehicle-to-vehicle scenarios

*Relevant ISO documents:*  
*ISO 26262-3: https://www.iso.org/standard/68385.html*  
*ISO 26262-6: https://www.iso.org/standard/68388.html*

## How to run
This project has been developed using Vector CANoe 17 SP2 (Build 17.2.88) and may not work on older versions.

To open the project, you must first load the configuration in CANoe:

![Load Config](https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/cf9cf0af-ba7b-4648-9388-592252a09d5f)

When opened for the first time, the panels may not be displayed correctly. This can be resolved by first opening the panel editor:

![Panels](https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/4ab94cc1-4f15-42a7-9cf6-f2708ece6486)

Inside the panel editor, change the background image as appropriate:

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/d9179f98-7efe-45ff-9e7a-3655d3ffcba3" alt="Edit panel">
</div>

The background images can be found in 'Panel Images,' inside the 'Driver Awareness System' folder:

![GitHub Panel Images](https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/6ecc8cd2-54f2-45a4-aafc-bc9bee1bd336)

When opened, the simulation can be run and stopped by using the appropriate buttons:

![Start Stop](https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/1afc4d5c-1dfc-4489-ba98-e69ec2706202)

<div align="center">
    <img src="https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/81a2604d-50f4-4b0c-84f2-7c003b39c87a" alt="CANoe running">
</div>

If you would like to change the scenario, open the Scenario Manager (make sure that 'Start on measurement start' is enabled) and click 'Load scenario':

![Scenario Manager](https://github.com/DeanLonergan/Driver_Awareness_System/assets/74914758/f0a804d3-7267-4a92-88f5-bd860debde5a)

Any of the three SETU scenarios can now be run. You can also create your own scenarios using the scenario editor.



