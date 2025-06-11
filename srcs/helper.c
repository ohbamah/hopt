#include "../includes/__hopt_.h"

inline
int
itCanBeRedefined(int n)
{
	return (i_hopt_redef_allowed == TRUE || (i_hopt_maps[n].flag & HOPT_FLSEQ));
}

inline
int
itCanBeOverwritable(int n)
{
	return (i_hopt_redef_overwrt == TRUE || (i_hopt_maps[n].flag & HOPT_FLFLOW));
}

inline
int
itCanBeRedefOvwr(int n)
{
	return (itCanBeOverwritable(n) && itCanBeRedefined(n));
}
