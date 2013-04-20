import contourspectrum

bin_edges = []
n_bins = 32
for be in range(0, n_bins+1):
	bin_edges.append(be)

contourspectrum.set_spectrum(bin_edges)

