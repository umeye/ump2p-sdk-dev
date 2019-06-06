
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NPCUMSPSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NPCUMSPSERVER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef NPCUMSPSERVER_EXPORTS
#define NPCUMSPSERVER_API __declspec(dllexport)
#else
#define NPCUMSPSERVER_API __declspec(dllimport)
#endif

// This class is exported from the NpcUmspServer.dll
class NPCUMSPSERVER_API CNpcUmspServer {
public:
	CNpcUmspServer(void);
	// TODO: add your methods here.
};

extern NPCUMSPSERVER_API int nNpcUmspServer;

NPCUMSPSERVER_API int fnNpcUmspServer(void);

