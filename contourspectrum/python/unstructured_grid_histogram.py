#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# by Panos Mavrogiorgos, email: pmav99 <> gmail
from vtk import vtk
from vtk import vtkXMLUnstructuredGridReader
from vtk import vtkFloatArray
from vtk import vtkTable
from vtk import vtkAxis
from vtk import vtkChart
from vtk import vtkChartXY
from vtk import vtkContextView
import contourspectrum
 
# The source file
file_name = "F:/data/unstructured_grids/spx.vtu"
 
# Read the source file.
reader = vtkXMLUnstructuredGridReader()
reader.SetFileName(file_name)
reader.Update() # Needed because of GetScalarRange
output = reader.GetOutput()
scalar_range = output.GetScalarRange()

data = output.GetPointData().GetArray(0)
scalar_range = data.GetValueRange()
n_bins = 32
bin_interval = (scalar_range[1] - scalar_range[0])/n_bins
histogram = vtkFloatArray()
histogram.SetNumberOfValues(n_bins + 1)
bin_edges = vtkFloatArray()
bin_edges.SetNumberOfValues(n_bins + 1)
for b in range(0, n_bins + 1):
    histogram.SetValue(b, 0)
    bin_edges.SetValue(b, b * bin_interval + scalar_range[0])

# Build the histogram
for p in range(0, output.GetNumberOfPoints()):
    value = data.GetValue(p)
    value_bin =  (value - scalar_range[0])/bin_interval
    value_bin = int(value_bin)
    value_bin = min([value_bin, n_bins - 1]);
    histogram.SetValue(value_bin, histogram.GetValue(value_bin) + 1.0)

# Or generate the contour spectrum
py_bin_edges = [];
for bi in range(0, n_bins + 1):
    py_bin_edges.append(bin_edges.GetValue(bi))
contourspectrum.set_spectrum(py_bin_edges)    
 
coord = []
for ci in range(0, 3):
    coord.append(0.0)
         
for ci in range(0, output.GetNumberOfCells()):
    cell = output.GetCell(ci)
    n_pts = cell.GetNumberOfPoints()
    scalars = []
    coords = []
    for p in range(0, n_pts):
        ptid = cell.GetPointId(p)
        coords.append(list(output.GetPoints().GetPoint(ptid)))
        scalars.append(data.GetValue(ptid))
    contourspectrum.add_tetra(coords, scalars)

# Setup contour spectrum
py_spectrum = [];
for bi in range(0, n_bins):
    py_spectrum.append(0.0)
contourspectrum.get_spectrum(py_spectrum)

spectrum = vtkFloatArray()
spectrum.SetNumberOfValues(n_bins+1)
for bi in range(0, len(py_spectrum)):
    spectrum.SetValue(bi, py_spectrum[bi])
     
chart = vtkChartXY()

histogram_table = vtkTable()
bin_edges.SetName("Bin Edges")
histogram_table.AddColumn(bin_edges)

histogram_pdf = vtkFloatArray()
histogram_pdf.SetNumberOfValues(n_bins+1)
histogram_pdf.SetName("PDF (Histogram)")
histogram_sum = 0;
for b in range(0, histogram.GetDataSize()):
    histogram_sum += histogram.GetValue(b)
for b in range(0, histogram.GetDataSize()):
    if(0 != histogram_sum ):
        histogram_pdf.SetValue(b, histogram.GetValue(b)/histogram_sum)
    else:    
        histogram_pdf.SetValue(b, 0.0)
histogram_table.AddColumn(histogram_pdf)

spectrum_pdf = vtkFloatArray()
spectrum_pdf.SetNumberOfValues(spectrum.GetDataSize())
spectrum_pdf.SetName("PDF (Contour Spectrum)")
spectrum_sum = 0;
for b in range(0, spectrum.GetDataSize()):
    spectrum_sum += spectrum.GetValue(b)
for b in range(0, spectrum.GetDataSize()):
    if(0 != spectrum_sum ):
        spectrum_pdf.SetValue(b, spectrum.GetValue(b)/spectrum_sum)
    else:   
        spectrum_pdf.SetValue(b, 0.0)
histogram_table.AddColumn(spectrum_pdf)

bar_plot =  chart.AddPlot(vtkChart.BAR)
if vtk.VTK_MAJOR_VERSION >= 6:
    bar_plot.SetInputData(histogram_table, 0, 1)
else:
    bar_plot.SetInput(histogram_table, 0, 1)

bar_plot =  chart.AddPlot(vtkChart.BAR)
if vtk.VTK_MAJOR_VERSION >= 6:
    bar_plot.SetInputData(histogram_table, 0, 2)
else:
    bar_plot.SetInput(histogram_table, 0, 2)

chart.GetAxis(vtkAxis.BOTTOM).SetMinimum(scalar_range[0])
chart.GetAxis(vtkAxis.BOTTOM).SetMaximum(scalar_range[1])
chart.GetAxis(vtkAxis.BOTTOM).SetTitle('Bin')
chart.GetAxis(vtkAxis.LEFT).SetTitle('Probability')
chart.SetTitle("PDF")
chart.SetShowLegend(True)

view = vtkContextView()
view.GetScene().AddItem(chart)
view.GetInteractor().Initialize()
view.GetInteractor().Start()

