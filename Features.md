# Features

- General
    - Controls
        - [x] Gamepad
        - [ ] *FPV radio controller*\*
        - [x] Keyboard
    - Flight
        - [x] Basic forces
        - [x] Drag
        - [ ] Turbulence
            - [ ] Propwash (like fall recovery stall/wobble)
    - Audio
        - [x] Rotors
        - [ ] Leaves
    - Utility
        - [x] Detection of being stuck
        - [x] Unflip
- Satisfactory
    - Equipment
        - Logic
            - [ ] Inventory
            - First person character actions
                - [ ] Place/pick up
                - [ ] Put googles on, take googles off
                - [ ] Line of sight flying mode when googles are off
        - Visuals
            - Icons
                - [ ] Drone
                - [ ] FPV controller
            - Models
                - [ ] Drone
                - [ ] Drone FPV view
                - [ ] *FPV controller*\*
                - [ ] *FPV googles*\*
            - Animations
                - *FPV controller actions*\*
                    - [ ] *Arm/disarm*\*
                    - [ ] *Throttle, yaw, pitch, roll*\*
                - *FPV googles*\*
                    - [ ] *Put googles on, take googles off*\*
            - Audio
                - [ ] *FPV controller*\*
                - [ ] *FPV googles*\*
    - Crafting
        - [ ] Drone recipe
        - [ ] FPV controller recipe
    - Radio coverage and interaction with buildings
        - Radio coverage
            - [ ] Limit to covered area
            - [ ] Loss of control when the retranslator chain is broken
        - [ ] Retranslator building (attaches to poles and power towers)
        - Hypertubes
            - [ ] Hypertube entrances are retranslators
            - [ ] Every segment that is connected to a powered entrance works as a small retranslator (to fly along or inside the tubes)
            - Flying inside the hypertubes
                - [ ] Third-person camera
                - [ ] First-person camera

\* - nobody to work on it

# What won't be added

- Other types of vehicles
    Should keep the mod simple for easier maintainability. FPV drone sims are doing well by just focusing on the FPV drones.
- Racing course buildings (gates, timers)
    Should be handled with a separate general purpose mod that would allow to build object detectors and computers.
- Simultaneous hybrid input: keyboard input for the character, controller for the drone
    Too complicated when trying to keep it from breaking by the full controller support in the main game.
