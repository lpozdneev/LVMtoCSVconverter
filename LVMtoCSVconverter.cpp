/*
    LVMtoCSVconverter.cpp
    File Converter to Convert .lvm Files to .csv
    Leonid Pozdneev
    January 2019

    This program takes all .lvm files in the directory where it's run, formats the data in those files,
    and record the data in .csv files with the same names.

    NOTES:  This program is designed to convert .lvm files created by Data Logger utility in NI ELVISmx.
            Data Logger formats data in the following way: "<date stamp><double space><time stamp><\t><measurment>".
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

    USAGE:  Place compiled .exe file of this .cpp into the directory with your .lvm files, and run the .exe (double click).


*/

#include <iostream>
#include <fstream>
#include <io.h>
#include <string>

using namespace std;

int main( int argc, char* argv[] )
{


    _finddata_t data;
    int ff = _findfirst ("./*.lvm", &data);

    if (ff != -1)
    {
        int res = 0;

        while (res != -1)
        {
            ofstream outfile;
            ifstream infile;

            string lvm_file = data.name;
            int dot = lvm_file.find_last_of(".");
            string csv_file = lvm_file.substr(0, dot) + ".csv";

            infile.open(lvm_file);
            outfile.open(csv_file);

            string initial;
            string converted;

            bool start_set = false;
            double start_time = 0;
            int i = 0;

            while (getline(infile, initial)) {
                if (i == 0) { // skip the first line
                    i++;
                } else {
                    // find the double space and record everything after it (throw away the date stamp)
                    int space = initial.find_first_of("  ");
                    converted = initial.substr(space + 1, initial.length());

                    // find the two colons used in the time stamp, and the tab after the time stamp
                    int colon1 = converted.find_first_of(":");
                    int colon2 = converted.find_last_of(":");
                    int tab = converted.find_first_of("\t");

                    // convert the time stamp into seconds
                    double hour = stod (converted.substr(0, colon1), nullptr);
                    double mins = stod (converted.substr(colon1 + 1, colon2), nullptr);
                    double secs = stod (converted.substr(colon2 + 1, tab), nullptr);
                    double total_secs = hour*3600 + mins*60 + secs;

                    // check if the start time as already been recorded
                    if (!start_set) {
                        start_time = total_secs;
                        start_set = true;
                    }

                    // replce the time stamp with the seconds value, and replace the tab with a comma
                    string time = to_string(total_secs - start_time);
                    converted.replace(0, (converted.substr(0, tab).length()), time);
                    tab = converted.find_first_of("\t");
                    converted.replace(tab, 1, ",");

                    outfile << converted << "\n";
                }
            }
            infile.close();
            outfile.close();
            res = _findnext(ff, &data);
        }
        _findclose(ff);
    }
    return 0;
}
