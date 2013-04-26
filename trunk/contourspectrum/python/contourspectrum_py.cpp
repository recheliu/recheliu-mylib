#include <iostream>
#include <list>
#include <vector>
using namespace std;
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/to_python_value.hpp>
#include "contourspectrum.h"

// Ref: http://stackoverflow.com/questions/10651912/pass-python-list-to-c-extension-using-boost-python

vector<double> vdBinEdges;
#if 0	// MOD-BY-LEETEN 04/26/2013-FROM:
#if	0	// MOD-BY-LEETEN 04/21/2013-FROM:
vector<double> vdSpectrum;
vector<double> vdTetraSpectrum;
#else	// MOD-BY-LEETEN 04/21/2013-TO:
unordered_map<size_t, double> hashSpectrum;
unordered_map<size_t, double> hashTetraSpectrum;
#endif	// MOD-BY-LEETEN 04/21/2013-END
#else	// MOD-BY-LEETEN 04/26/2013-TO:
vector<double> vdSpectrum;
size_t uFirstTetraSpectrumBin;
vector<double> vdTetraSpectrum;
#endif	// MOD-BY-LEETEN 04/26/2013-END
void set_spectrum
(
	boost::python::list& bin_edges
)
{
	size_t uNrOfBinEdges = (size_t)boost::python::len(bin_edges);
	vdBinEdges.resize(uNrOfBinEdges);
	for(size_t be = 0; be < uNrOfBinEdges; be++)
	{
		vdBinEdges[be] = boost::python::extract< double >(bin_edges[be]);
	}
	
	size_t uNrOfBin = uNrOfBinEdges - 1;
	#if	0	// TEST-MOD
	// MOD-BY-LEETEN 04/21/2013-FROM:	vdSpectrum.assign(uNrOfBin, 0.0);
	hashSpectrum.clear();
	// MOD-BY-LEETEN 04/21/2013-END
	#else
	vdSpectrum.assign(uNrOfBin, 0.0);
	#endif
}

void get_spectrum
(
	boost::python::list& spectrum
)
{
	#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
	#if	0	// MOD-BY-LEETEN 04/21/2013-FROM:
	for(size_t b = 0; b < (size_t)boost::python::len(spectrum); b++)
		spectrum[b] = vdSpectrum[b];	// boost::python::to_python_value< double >(vdSpectrum[b]);
	#else	// MOD-BY-LEETEN 04/21/2013-TO:
	for(unordered_map<size_t, double>::iterator
			ihashSpectrum = hashSpectrum.begin();
		ihashSpectrum != hashSpectrum.end();
		ihashSpectrum++)
		spectrum[ihashSpectrum->first] = ihashSpectrum->second;	// boost::python::to_python_value< double >(vdSpectrum[b]);
	#endif	// MOD-BY-LEETEN 04/21/2013-END
	#else	// MOD-BY-LEETEN 04/26/2013-TO:
	for(size_t b = 0; b < (size_t)boost::python::len(spectrum); b++)
		spectrum[b] = vdSpectrum[b];	// boost::python::to_python_value< double >(vdSpectrum[b]);
	#endif	// MOD-BY-LEETEN 04/26/2013-END
}

void add_tetra
(
	boost::python::list& coords, 
	boost::python::list& scalars
)
{
	// TODO: check that the length is 4
	
	// TODO: check that the length of the coords and the scalara are the same
	size_t uNrOfVertices = boost::python::len(coords);
		
	vector< pair<double, glm::dvec4> > vVertices;
	vVertices.resize(uNrOfVertices);
	
	//
	for(size_t vi = 0; vi < vVertices.size(); vi++)
	{
		boost::python::list ldCoord  = boost::python::extract< boost::python::list >(coords[vi]);
		vVertices[vi].first = boost::python::extract< double >(scalars[vi]);
		vVertices[vi].second = glm::vec4(0.0);
		for(size_t ci = 0; ci < (size_t)boost::python::len(ldCoord); ci++)
			vVertices[vi].second[ci] = boost::python::extract< double >(ldCoord[ci]);
	}

	#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
	#if 0 	// MOD-BY-LEETEN 04/21/2013-FROM:
	ContourSpectrum::_Compute(vdBinEdges, vVertices, vdTetraSpectrum);

	for(size_t b = 0; b < vdSpectrum.size(); b++)
		vdSpectrum[b] += vdTetraSpectrum[b];
	#else	// MOD-BY-LEETEN 04/21/2013-TO:
	ContourSpectrum::_Compute(vdBinEdges, vVertices, hashTetraSpectrum);
	for(unordered_map<size_t, double>::iterator
			ihashTetra = hashTetraSpectrum.begin();
		ihashTetra != hashTetraSpectrum.end();
		ihashTetra++)
	{
		unordered_map<size_t, double>::iterator ihashSpectrum = hashSpectrum.find(ihashTetra->first);
		if( ihashSpectrum == hashSpectrum.end() )
			hashSpectrum.insert(*ihashTetra);
		else
			ihashSpectrum->second += ihashTetra->second;
	}
	#endif	// MOD-BY-LEETEN 04/21/2013-END
	#else	// MOD-BY-LEETEN 04/26/2013-TO:
	ContourSpectrum::_Compute(vdBinEdges, vVertices, uFirstTetraSpectrumBin, vdTetraSpectrum);

	for(size_t b = uFirstTetraSpectrumBin; b < vdSpectrum.size(); b++)
	{
		if( !vdTetraSpectrum[b] )
			break;
		vdSpectrum[b] += vdTetraSpectrum[b];
	}
	#endif	// MOD-BY-LEETEN 04/26/2013-END
}

BOOST_PYTHON_MODULE(contourspectrum)
{
    using namespace boost::python;
	def<void(
		boost::python::list& coords, 
		boost::python::list& scalars
	)>("add_tetra", add_tetra);
	def<void(
		boost::python::list&
	)>("set_spectrum", set_spectrum);
	def<void(
		boost::python::list&
	)>("get_spectrum", get_spectrum);
}



