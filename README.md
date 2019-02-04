# LVMtoCSVconverter

This program is designed to convert .lvm files created by Data Logger utility in NI ELVISmx (software for NI myDAQ).
Data Logger formats data in the following way: "< datestamp >< doublespace >< timestamp ><\t>< measurment >". This program works for data collected from either 1 or 2 inputs.
The program WILL NOT WORK CORRECTLY if your .lvm files are formatted differently.

ONLY WORKS ON WINDOWS 64-BITS (32-bit systems have not been tested).

This is how data is formatted in .csv files:
1) Tabs in the first column are replaced with commas.
2) The date stamp is removed. (So if during your measurments the date changes, the times in the .csv file will not be correct,
but that can be solved when you are processing your data.)
3) The time stamp is converted into seconds, and the first measurment is taken as 0 seconds.
(e.g. if your first measurment happened at 12:34:56.7, and the second measurment happened at 12:34:57.8, then in the .csv
file, the first measurment will have time 0, and the second measurment will have time 1.1.)
4) Columns are separated by commas.

To use the converter, you need to put LVMtoCSVconverter.exe file into the directory with .lvm files that you want to convert and run the .exe. If Windows gives you a warning, you need to click "More Info", and then "Run Anyways" (or you can just complie the .cpp file).
