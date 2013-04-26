#pragma once

#include <vector>
#include <unordered_map>	// ADD-BY-LEETEN 04/21/2013
#include <algorithm>
using namespace std;
#include <glm/glm.hpp>
#include <glm/core/func_matrix.hpp>
#include <glm/core/type_mat3x3.hpp>
#include <glm/core/func_vector_relational.hpp>

namespace ContourSpectrum 
{
	bool 
	BCompVec(
		const pair<double, glm::dvec4>& pairI,
		const pair<double, glm::dvec4>& pairJ
		)
	{	
		if( pairI.first < pairJ.first )
			return true;
		if( pairI.first > pairJ.first )
			return false;
		for(size_t d = 0; d < 4; d++)
		{
			if(pairI.second[d] < pairJ.second[d])
				return true;
			if(pairI.second[d] > pairJ.second[d])
				return false;
		}
		return false;
	}

	//! Compute the contour spectrum of a simplex. Currently only 2D and 3D simplices are supported.	
	void
	_Compute(
		const vector<double>& vdBinEdges,
		vector<pair<double, glm::dvec4>>& vVertices,
		#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
		// MOD-BY-LEETEN 04/21/2013-FROM:			vector<double>& vdHistogram,
		unordered_map<size_t, double>& hashHistogram,
		// MOD-BY-LEETEN 04/21/2013-END
		#else	// MOD-BY-LEETEN 04/26/2013-TO:
		size_t& uFirstBin,
		vector<double>& vdHistogram,
		#endif	// MOD-BY-LEETEN 04/26/2013-END
		void *_Reserved = NULL
	)
	{
		size_t uNrOfBins = vdBinEdges.size() - 1;
		#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
		// MOD-BY-LEETEN 04/21/2013-FROM:			vdHistogram.assign(uNrOfBins, 0.0);
		hashHistogram.clear();
		// MOD-BY-LEETEN 04/21/2013-END
		#else	// MOD-BY-LEETEN 04/26/2013-TO:
		vdHistogram.assign(uNrOfBins, 0.0);
		uFirstBin = uNrOfBins;
		#endif	// MOD-BY-LEETEN 04/26/2013-END

		size_t uNrOfVertices = vVertices.size();
		size_t uNrOfDims = uNrOfVertices - 1;
		size_t uNrOfStates = uNrOfDims;
		
		// sort V and X based on V
		sort(vVertices.begin(), vVertices.end(), BCompVec);

		// for the degnerated case that all vertices have the same value, immediately update the bin and then return
		double dVol = 0.0;
		switch(uNrOfDims)
		{
			case 2: {
				// vol  = abs(det([X(1, :),   1; X(2, :), 1; X(3, :), 1]))/2;
				dVol = fabs(glm::determinant(glm::dmat3x3(
					vVertices[0].second[0], vVertices[0].second[1], 1.0,
					vVertices[1].second[0], vVertices[1].second[1], 1.0,
					vVertices[2].second[0], vVertices[2].second[1], 1.0)))/2.0;
			} break;
			case 3: {
				dVol = fabs(glm::determinant(glm::dmat3x3(
					glm::vec3(vVertices[1].second - vVertices[0].second),
					glm::vec3(vVertices[2].second - vVertices[0].second),
					glm::vec3(vVertices[3].second - vVertices[0].second))))/6.0;
			} break;
		}
		bool bAreAllVerticesTheSame = true;
		for(size_t v = 1; v < vVertices.size(); v++)
			if( vVertices[v].first != vVertices[0].first )
			{
				bAreAllVerticesTheSame = false;
				break;
			}
		if(bAreAllVerticesTheSame )
		{
			for(size_t b = 0; b < uNrOfBins; b++)
				if( vdBinEdges[b] <= vVertices[0].first && vVertices[0].first < vdBinEdges[b+1] )
				{
					#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
					// MOD-BY-LEETEN 04/21/2013-FROM:						vdHistogram[b] += dVol;
					hashHistogram.insert(make_pair<size_t, double>(b, dVol));
					// MOD-BY-LEETEN 04/21/2013-END
					#else	// MOD-BY-LEETEN 04/26/2013-TO:
					vdHistogram[b] += dVol;
					uFirstBin = b;
					#endif	// MOD-BY-LEETEN 04/26/2013-END
					break;
				}
			return;
		}
		
		// if( 3 == n_states && V_sorted(1) == V_sorted(2) && V_sorted(2) ~= V_sorted(3) && V_sorted(3) == V_sorted(4) )
		if( 3 == uNrOfStates && vVertices[0].first == vVertices[1].first && vVertices[1].first != vVertices[2].first && vVertices[2].first == vVertices[3].first )
		{
			glm::dvec4 vMiddle = (vVertices[1].second + vVertices[2].second)/2.0;
			double dMiddle = (vVertices[1].first + vVertices[2].first)/2.0;
			#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
			// MOD-BY-LEETEN 04/21/2013-FROM:				vector<double> pvdHistograms[2];
			unordered_map<size_t, double> phashHistograms[2];
			// MOD-BY-LEETEN 04/21/2013-END
			#else	// MOD-BY-LEETEN 04/26/2013-TO:
			size_t puFirstBins[2];
			vector<double> pvdHistograms[2];
			#endif	// MOD-BY-LEETEN 04/26/2013-END
			vector< pair<double, glm::dvec4> > pvVertices[2];

			pvVertices[0].assign(vVertices.begin(), vVertices.end());
			pvVertices[0][2].first = dMiddle;
			pvVertices[0][2].second = vMiddle;
			_Compute(
				vdBinEdges,
				pvVertices[0],
				#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
				// MOD-BY-LEETEN 04/21/2013-FROM:					pvdHistograms[0]
				phashHistograms[0]
				// MOD-BY-LEETEN 04/21/2013-END
				#else	// MOD-BY-LEETEN 04/26/2013-TO:
				puFirstBins[0],
				pvdHistograms[0]
				#endif	// MOD-BY-LEETEN 04/26/2013-END
			);

			pvVertices[1].assign(vVertices.begin(), vVertices.end());
			pvVertices[1][1].first = dMiddle;
			pvVertices[1][1].second = vMiddle;
			_Compute(
				vdBinEdges,
				pvVertices[1],
				#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
				// MOD-BY-LEETEN 04/21/2013-FROM:					pvdHistograms[1]
				phashHistograms[1]
				// MOD-BY-LEETEN 04/21/2013-END
				#else	// MOD-BY-LEETEN 04/26/2013-TO:
				puFirstBins[1],
				pvdHistograms[1]
				#endif	// MOD-BY-LEETEN 04/26/2013-END
			);

			#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
			#if	0	// MOD-BY-LEETEN 04/21/2013-FROM:	
			for(size_t b = 0; b < uNrOfBins; b++)
				vdHistogram[b] = pvdHistograms[0][b] + pvdHistograms[1][b]; 
			#else	// MOD-BY-LEETEN 04/21/2013-TO:
			for(size_t h = 0; h < 2; h++)
			{
				for(unordered_map<size_t, double>::iterator 
						ihash = phashHistograms[h].begin();
					ihash != phashHistograms[h].end();
					ihash++)
				{
					unordered_map<size_t, double>::iterator ihashDst = hashHistogram.find(ihash->first);
					if( ihashDst != hashHistogram.end() )
						ihashDst->second += ihash->second;
					else
						hashHistogram.insert(*ihash);
				}
			}
			#endif	// MOD-BY-LEETEN 04/21/2013-END
			#else	// MOD-BY-LEETEN 04/26/2013-TO:

			uFirstBin = min(puFirstBins[0], puFirstBins[1]);
			for(size_t b = uFirstBin; b < uNrOfBins; b++)
			{
				vdHistogram[b] = pvdHistograms[0][b] + pvdHistograms[1][b]; 
				if( !vdHistogram[b] )
					break;
			}
			#endif	// MOD-BY-LEETEN 04/26/2013-END
			return;
		}

		vector<double> vdIntegrals;
		vdIntegrals.assign(uNrOfStates, 0.0);

		switch (uNrOfStates)
		{
		case 2: {
				double r = (vVertices[1].first - vVertices[0].first)/(vVertices[2].first - vVertices[0].first);
				glm::dvec4 vMiddle = vVertices[0].second * (1.0 - r) + vVertices[2].second * r;
				vdIntegrals[0] = fabs(
					glm::determinant(
						glm::dmat3x3(
							vVertices[0].second[0], vVertices[0].second[1], 1.0,
							vVertices[1].second[0], vVertices[1].second[1], 1.0,
							vMiddle[0],				vMiddle[1],				1.0)))/2.0;	
				vdIntegrals[1] = fabs(
					glm::determinant(
						glm::dmat3x3(
							vVertices[1].second[0], vVertices[1].second[1], 1.0,
							vVertices[2].second[0], vVertices[2].second[1], 1.0,
							vMiddle[0],				vMiddle[1],				1.0)))/2.0;	
		} break;
		case 3: {
			glm::dvec4
				Xa = vVertices[0].second,
				Xb = vVertices[1].second,
				Xc = vVertices[2].second,
				Xd = vVertices[3].second;
			double 
				a = vVertices[0].first,
				b = vVertices[1].first,
				c = vVertices[2].first,
				d = vVertices[3].first;
			if( a != c && d != a )
			{
				double rb_ac = (b - a)/(c - a);
				glm::dvec4 Xb_ac = Xa * (1.0 - rb_ac) + Xc * rb_ac;
				double rb_ad = (b - a)/(d - a);
				glm::dvec4 Xb_ad = Xa * (1.0 - rb_ad) + Xd * rb_ad;
				vdIntegrals[0] = fabs(
					glm::determinant(
						glm::dmat3x3(
							glm::vec3(Xb - Xa), 
							glm::vec3(Xb_ac - Xa), 
							glm::vec3(Xb_ad - Xa))))/6.0;
			}

			if( b != a )
			{
				double rc_ab = (c - a) / (b - a);
				glm::dvec4 Xc_ab = Xa * (1.0 - rc_ab) + Xb * rc_ab;
				double rc_bd = (c - b) / (d - b);
				glm::dvec4 Xc_bd = Xb * (1.0 - rc_bd) + Xd * rc_bd;
				vdIntegrals[1] = fabs(
					glm::determinant(
						glm::dmat3x3(
							glm::vec3(Xc - Xb), 
							glm::vec3(Xc_ab - Xb), 
							glm::vec3(Xc_bd - Xb))))/6.0;
			}
			else
			{
				double rb_cd = (b - c) / (d - c);
				glm::dvec4 Xb_cd = Xc * (1.0 - rb_cd) + Xd * rb_cd;
				double rb_ad = (b - a)/(d - a);
				glm::dvec4 Xb_ad = (1 - rb_ad) * Xa + rb_ad * Xd;
				vdIntegrals[1] = fabs(
					glm::determinant(
						glm::dmat3x3(
							glm::vec3(Xc - Xb), 
							glm::vec3(Xb_cd - Xb), 
							glm::vec3(Xb_ad - Xb))))/6.0;
			}
			
			if( d != a && d != b )
			{
				double rc_ad = (c - a)/(d - a);
				glm::dvec4 Xc_ad = Xa * (1.0 - rc_ad) + Xd * rc_ad;
				double rc_bd = (c - b)/(d - b);
				glm::dvec4 Xc_bd = Xb * (1.0 - rc_bd) + Xd * rc_bd;
				vdIntegrals[2] = fabs(
					glm::determinant(
						glm::dmat3x3(
							glm::vec3(Xc - Xd), 
							glm::vec3(Xc_ad - Xd), 
							glm::vec3(Xc_bd - Xd))))/6.0;
			}
		} break;
		}
		
		for(size_t state = 0; state < uNrOfStates; state++)
		{
			size_t uBinMin = uNrOfBins;
			size_t uBinMax = uNrOfBins;
			for(size_t b = 0; b < uNrOfBins; b++)
			{
				if( vdBinEdges[b] <= vVertices[state].first )
					uBinMin = b;
				if( vdBinEdges[b] <= vVertices[state+1].first )
					uBinMax = b;
			}
			if( uNrOfBins == uBinMin && uNrOfBins == uBinMax  )
				continue;
			if( uNrOfBins == uBinMin ) 
				uBinMin = 0;
			if( uNrOfBins == uBinMax ) 
				uBinMax = uNrOfBins - 1;
			for(size_t b = uBinMin; b <= uBinMax; b++)
			{
				double v0 = max(vdBinEdges[b], vVertices[state].first);
				double v1 = min(vdBinEdges[b+1], vVertices[state+1].first);
				double dNewContrib = 0;
				if( vVertices[state].first == vVertices[state+1].first )
					continue;
				if( !state )
				{
					double ratio0 = (v0 - vVertices[state].first)/(vVertices[state+1].first - vVertices[state].first);
					double dIntegral0 = vdIntegrals[state] * pow(ratio0, (double)uNrOfDims);
					double ratio1 = (v1 - vVertices[state].first)/(vVertices[state+1].first - vVertices[state].first);
					double dIntegral1 = vdIntegrals[state] * pow(ratio1, (double)uNrOfDims);
					dNewContrib = dIntegral1 - dIntegral0;
				}
				else 
				if( uNrOfStates - 1 == state )
				{
					double ratio0 = (v0 - vVertices[state+1].first)/(vVertices[state].first - vVertices[state+1].first);
					double Integral0 = vdIntegrals[state] * pow(ratio0, (double)uNrOfDims);
					double ratio1 = (v1 - vVertices[state+1].first)/(vVertices[state].first - vVertices[state+1].first);
					double Integral1 = vdIntegrals[state] * pow(ratio1, (double)uNrOfDims);
					dNewContrib = Integral0 - Integral1;
				}
				else
				{
					if( vVertices[state].first != vVertices[state-1].first  )
					{
						double ratio0, ratio1;
						double V1r, V2r;
						ratio0 = (v0 - vVertices[state-1].first)/(vVertices[state].first - vVertices[state-1].first);
						V1r = vdIntegrals[0] * pow(ratio0, (double)uNrOfDims);
						ratio0 = (v0 - vVertices[state].first)/(vVertices[state + 1].first - vVertices[state].first);
						V2r = vdIntegrals[1] * pow(ratio0, (double)uNrOfDims);
						double Integral0 = V1r - V2r;

						ratio1 = (v1 - vVertices[state - 1].first)/(vVertices[state].first - vVertices[state-1].first);
						V1r = vdIntegrals[0] * pow(ratio1, (double)uNrOfDims);
						ratio1 = (v1 - vVertices[state].first)/(vVertices[state + 1].first - vVertices[state].first);
						V2r = vdIntegrals[1] * pow(ratio1, (double)uNrOfDims);
						double Integral1 = V1r - V2r;

						dNewContrib = Integral1 - Integral0;
					}
					else
					{
						double ratio0, ratio1;
						double V1r, V2r;
						ratio0 = (v0 - vVertices[3].first)/(vVertices[2].first - vVertices[3].first);
						V1r = vdIntegrals[2] * pow(ratio0, (double)uNrOfDims);
						ratio0 = (v0 - vVertices[2].first)/(vVertices[1].first - vVertices[2].first);
						V2r = vdIntegrals[1] * pow(ratio0, (double)uNrOfDims);
						double Integral0 = V1r - V2r;

						ratio1 = (v1 - vVertices[3].first)/(vVertices[2].first - vVertices[3].first);
						V1r = vdIntegrals[2] * pow(ratio1, (double)uNrOfDims);
						ratio1 = (v1 - vVertices[2].first)/(vVertices[1].first - vVertices[2].first);
						V2r = vdIntegrals[1] * pow(ratio1, (double)uNrOfDims);
						double Integral1 = V1r - V2r;

						dNewContrib = Integral0 - Integral1;
					}
				}
				#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
				// MOD-BY-LEETEN 04/21/2013-FROM:					vdHistogram[b] += dNewContrib;
				unordered_map<size_t, double>::iterator ihashDst = hashHistogram.find(b);
				if( ihashDst != hashHistogram.end() )
					ihashDst->second += dNewContrib;
				else
					hashHistogram.insert(make_pair<size_t, double>(b, dNewContrib));
				// MOD-BY-LEETEN 04/21/2013-END
				#else	// MOD-BY-LEETEN 04/26/2013-TO:
				vdHistogram[b] += dNewContrib;
				uFirstBin = min(uFirstBin, b);
				#endif	// MOD-BY-LEETEN 04/26/2013-END
			}
		}
	}

	void
	_ComputeFor3DCell
	(
		const vector<double>& vdBinEdges,
		const vector<pair<double, glm::dvec4> >& vCorners,
		#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
		// MOD-BY-LEETEN 04/21/2013-FROM:			vector<double>& vdHistogram,
		unordered_map<size_t, double>& hashHistogram,
		// MOD-BY-LEETEN 04/21/2013-END
		#else	// MOD-BY-LEETEN 04/26/2013-TO:
		size_t& uFirstBin,
		vector<double>& vdHistogram,
		#endif	// MOD-BY-LEETEN 04/26/2013-END
		size_t uTetraOrientation,
		void *_Reserved = NULL
	)
	{
		const static size_t pppuTetrahedraIndices[2][5][4] = 
		{
			{
				{0, 1, 3, 5},
				{0, 4, 5, 6},
				{3, 5, 6, 7},
				{0, 2, 3, 6},
				{0, 3, 5, 6}
			},
			{
				{1, 4, 5, 7},
				{0, 1, 2, 4},
				{1, 2, 3, 7},
				{2, 4, 6, 7},
				{1, 2, 4, 7}
			}
		};

		vector<pair<double, glm::dvec4> > vVertices;
		vVertices.resize(4);

		#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
		// MOD-BY-LEETEN 04/21/2013-FROM:			vdHistogram.assign(vdBinEdges.size()-1, 0.0);
		hashHistogram.clear();
		// MOD-BY-LEETEN 04/21/2013-END
		#else	// MOD-BY-LEETEN 04/26/2013-TO:
		vdHistogram.assign(vdBinEdges.size()-1, 0.0);
		uFirstBin = vdHistogram.size();
		#endif	// MOD-BY-LEETEN 04/26/2013-END
		for(size_t t = 0; t < 5; t++)
		{
			#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
			#if	0	// MOD-BY-LEETEN 04/21/2013-FROM:	
			vector<double> vdTetraHistogram;
			vdTetraHistogram.assign(vdBinEdges.size()-1, 0.0);
			#else	// MOD-BY-LEETEN 04/21/2013-TO:	
			unordered_map<size_t, double> hashTetraHistogram;
			#endif	// MOD-BY-LEETEN 04/21/2013-END
			#else	// MOD-BY-LEETEN 04/26/2013-TO:
			size_t uFirstTetraBin;
			vector<double> vdTetraHistogram;
			vdTetraHistogram.assign(vdBinEdges.size()-1, 0.0);
			#endif	// MOD-BY-LEETEN 04/26/2013-END
			for(size_t p = 0; p < 4; p++)
				vVertices[p] = vCorners[pppuTetrahedraIndices[uTetraOrientation][t][p]];
			#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
			#if	0	// MOD-BY-LEETEN 04/21/2013-FROM:	
			_Compute(
				vdBinEdges,
				vVertices,
				vdTetraHistogram);
			for(size_t b = 0; b < vdTetraHistogram.size(); b++)
				vdHistogram[b] += vdTetraHistogram[b];
			#else	// MOD-BY-LEETEN 04/21/2013-TO:	
			_Compute(
				vdBinEdges,
				vVertices,
				hashTetraHistogram);
			for(unordered_map<size_t, double>::iterator 
					ihashTetra = hashTetraHistogram.begin();
				ihashTetra != hashTetraHistogram.end();
				ihashTetra++)
			{
				unordered_map<size_t, double>::iterator 
					ihashDst = hashHistogram.find(ihashTetra->first);
				if( ihashDst != hashHistogram.end() )
					ihashDst->second += ihashTetra->second;
				else
					hashHistogram.insert(*ihashTetra);
			}
			#endif	// MOD-BY-LEETEN 04/21/2013-END
			#else	// MOD-BY-LEETEN 04/26/2013-TO:
			_Compute(
				vdBinEdges,
				vVertices,
				uFirstTetraBin,
				vdTetraHistogram);
			for(size_t b = uFirstTetraBin; b < vdTetraHistogram.size(); b++)
			{
				if(0.0 == vdTetraHistogram[b])
					break;
				vdHistogram[b] += vdTetraHistogram[b];
			}
			uFirstBin = min(uFirstBin, uFirstTetraBin);
			#endif	// MOD-BY-LEETEN 04/26/2013-END
		}
	}
	// ADD-BY-LEETEN 04/21/2013-BEGIN
	void
	_ComputeFor2DCell
	(
		const vector<double>& vdBinEdges,
		const vector<pair<double, glm::dvec4> >& vCorners,
		// MOD-BY-LEETEN 04/26/2013-FROM:		unordered_map<size_t, double>& hashHistogram,
		size_t& uFirstBin,
		vector<double>& vdHistogram,
		// MOD-BY-LEETEN 04/26/2013-END
		size_t uOrientation,
		void *_Reserved = NULL
	)
	{
		const static size_t pppuSimplexIndices[2][2][3] = 
		{
			{
				{0, 1, 2},
				{1, 3, 2}
			},
			{
				{0, 1, 3},
				{0, 3, 2}
			}
		};

		vector<pair<double, glm::dvec4> > vVertices;
		vVertices.resize(3);

		// MOD-BY-LEETEN 04/26/2013-FROM:		hashHistogram.clear();
		vdHistogram.assign(vdBinEdges.size()-1, 0.0);
		uFirstBin = vdHistogram.size();
		// MOD-BY-LEETEN 04/26/2013-END
		for(size_t t = 0; t < 2; t++)
		{
			// MOD-BY-LEETEN 04/26/2013-FROM:			unordered_map<size_t, double> hashTetraHistogram;
			size_t uFirstTetraBin;
			vector<double> vdTetraHistogram;
			vdTetraHistogram.assign(vdBinEdges.size()-1, 0.0);
			// MOD-BY-LEETEN 04/26/2013-END
			for(size_t p = 0; p < 3; p++)
				vVertices[p] = vCorners[pppuSimplexIndices[uOrientation][t][p]];
			#if	0	// MOD-BY-LEETEN 04/26/2013-FROM:
			_Compute(
				vdBinEdges,
				vVertices,
				hashTetraHistogram);
			for(unordered_map<size_t, double>::iterator 
					ihashTetra = hashTetraHistogram.begin();
				ihashTetra != hashTetraHistogram.end();
				ihashTetra++)
			{
				unordered_map<size_t, double>::iterator 
					ihashDst = hashHistogram.find(ihashTetra->first);
				if( ihashDst != hashHistogram.end() )
					ihashDst->second += ihashTetra->second;
				else
					hashHistogram.insert(*ihashTetra);
			}
			#else	// MOD-BY-LEETEN 04/26/2013-TO:
			_Compute(
				vdBinEdges,
				vVertices,
				uFirstTetraBin,
				vdTetraHistogram);
			for(size_t b = uFirstTetraBin; b < vdTetraHistogram.size(); b++)
			{
				if(0.0 == vdTetraHistogram[b])
					break;
				vdHistogram[b] += vdTetraHistogram[b];
			}
			uFirstBin = min(uFirstBin, uFirstTetraBin);
			#endif	// MOD-BY-LEETEN 04/26/2013-END
		}
	}
	// ADD-BY-LEETEN 04/21/2013-END
};

