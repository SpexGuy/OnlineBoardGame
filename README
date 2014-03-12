

Steps for creating a Visual Studio Project:
  1.  Go to File->New Project.
  2.  Select "Empty Project".
  3.  Give it the name "OBGCore" and call the solution "OnlineBoardGame".
  4.  From OBGCore, right-click->Properties
  5.  Change configuration to All Configurations
  6.  Go to Configuration Properties->General
  7.  Change Project Defaults.Configuration Type to Utility
  8.  Right-click the solution, and go to Add->New Project
  9.  Call the project "OBGGraphics"
  10. Repeat steps 4-7 for OBGGraphics
  11. Right-click the solution, and go to Add->New Project
  12. Call the new project "OBGClient"
  13. Make another project the same way called "OBGServer"
  14. Right-click OBGClient and go to Project Dependencies
  15. Check OBGCore
  16. Check OBGGraphics (OBGClient only)
  17. Right-click OBGClient->Properties
  18. Switch the configuration to All Configurations
  19. Navigate to Configuration Properties->VC++ Directories
  20. Hilight Include Directories and click edit
  21. Add the OBGCore directory
  22. Add the OBGGraphics directory (OBGClient only)
  23. Save
  24. Repeat steps 11-23 for OBGServer
  25. Download glm, freeglut, DevIL, and glew.
The directories you put them in will henceforth be denoted as
$GLM, $FREEGLUT, $DEVIL, and $GLEW, respectively.
  26. From OBGGraphics, right-click->Properties
  27. Switch to All Configurations
  28. Go to Configuration Properties->VC++ Directories
  29. Add to Include Directories the following:
        $GLEW\include
        $DEVIL\include
        $GLM
        $FREEGLUT\include
  30. Add to Library Directories the following:
        $DEVIL\lib
        $FREEGLUT\lib
        $GLEW\lib\Release\Win32
  31. Open OBGClient properties and go to C/C++->General
  32. Add $GLM to Additional Include Directories
  33. Go to Linker->General
  34. Add to Additional Library Directories:
        $DEVIL\lib
        $FREEGLUT\lib
        $GLEW\lib\Release\Win32
  35. Under Linker->Input, edit Additional Dependencies
  36. Add freeglut.lib, DevIL.lib, ILU.lib, ILUT.lib, and glew32.lib
  37. Find freeglut.dll, DevIL.dll, ILU.dll, ILUT.dll, and glew32.dll
  38. Copy these libraries to OnlineBoardGame\Debug (where the executable will be).
  39. Add the code files to the directories
  40. For each project, right-click->Add->Existing Item
  41.   Hilight all source and header files and click ok
  42. Congratulations!

