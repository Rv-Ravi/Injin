#ifndef COREINIT_H
#define COREINIT_H

#ifdef INJIN_DLL
	#define NJIN __declspec(dllexport)
#else
	#define NJIN __declspec(dllimport)
#endif //INJIN_DLL


#endif //COREINIT_H
