# PX4 Drone Autopilot for secure mavlink
This repository(branch) contains 'secure mavlink' implementation.
Based version is [v1.15.0](https://github.com/Lumy0726/PX4-Autopilot/releases/tag/v1.15.0/).
Please see the below link for the original README document.

[README](./README_orig.md/)

# Secure mavlink
Below list is implementations to enhance security of mavlink protocol.  
* Apply payload encryption of MAVLink protocol.

## Payload encryption of MAVLink protocol
Below list is implementations to apply payload encryption.
* Modify 'src/modules/mavlink/mavlink' submodule commit, to use encryption supported MAVLink protocol.  
Actual implementation is on [pymavlink](https://github.com/Lumy0726/pymavlink/tree/securemav/) submodule.  
* To use AES128 CTR encryption method, include external library.  
[original library implementation branch](https://github.com/Lumy0726/PX4-Autopilot/tree/mesl_lib/)  
[library implementation branch, rebased to v1.15.0](https://github.com/Lumy0726/PX4-Autopilot/tree/mesl_lib_rbd/)  
[crypto module directory](./src/modules/mesl_crypto/)  
And disable 'se' driver of external library, from build configuration.
* Modify 'mavlink' module of PX4 to enable encryption.  
[mavlink module directory](./src/modules/mavlink/)  
Encryption can be enabled, using startup scripts. This can be configured per MAVLink channel. Therefore startup script and 'cdcacm_autostart' driver also has been modified.  
Some debugging code also has been implemented, and can be toggled using C macro variable.  

