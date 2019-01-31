# LVMtoCSVconverter
A simple program that converts .lvm files created by Data Logger utility in NI myDAQ software into .csv files.

This program is designed to convert .lvm files created by Data Logger utility in NI ELVISmx.
Data Logger formats data in the following way: "< datestamp >< doublespace >< timestamp ><\t>< measurment >".
The program WILL NOT WORK CORRECTLY if your .lvm files are formatted differently.

ONLY WORKS ON WINDOWS 64-BITS (32-bit systems have not been tested).

This is how data is formatted in .csv files:
1) The first line with column names is remmoved (e.g. "Time  myDAQ1 (NI myDAQ)/ai0").
2) The date stamp is removed. (So if during your measurments the date changes, the times in the .csv file will not be correct,
but that can be solved when you are processing your data.)
3) The time stamp is converted into seconds, and the first measurment is taken as 0 seconds.
(e.g. if your first measurment happened at 12:34:56.7, and the second measurment happened at 12:34:57.8, then in the .csv
file, the first measurment will have time 0, and the second measurment will have time 1.1.)
4) Time column and the column with the measured value are separated by a comma.

Place compiled .exe file of this .cpp into the directory with your .lvm files, and run the .exe (double click).
