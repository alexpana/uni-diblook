newoption{
	trigger = "destination", 
	description = "Destination folder"
}

-- Root directory of project checkout relative to this .lua file
rootdir = ".."

-- Create the solution and make it active
solution "diblook"
	configurations{ "Debug", "Release" }
	location( _OPTIONS["destination"] )


-- Create the application project
project "diblook"
	language "C++"
	kind "WindowedApp"
	targetdir( rootdir.."/binaries" )
	
	resincludedirs{ rootdir.."/res" }
	
	flags { "StaticRuntime", "WinMain" }
    
	files{
		rootdir.."/source/include/**.h",
		rootdir.."/source/include/*.rc",
		rootdir.."/source/src/**.cpp"
	}
	
	includedirs{
		rootdir.."/source/include"
	}
	
	configuration "Debug"
		defines { "DEBUG" }
		targetname "diblookd"
		flags {"Symbols"} -- enable debug information

	configuration "Release"
		defines { "NDEBUG" }
		targetname "diblook"