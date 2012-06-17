#ifdef FCEXPORTIMPL
#define FCEXPORT __declspec(dllexport)
#else
#define FCEXPORT __declspec(dllimport)
#endif
