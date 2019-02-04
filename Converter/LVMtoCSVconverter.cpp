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

void convert_time(string *s, bool *start_set, double *start_time) {
    // find the double space and record everything after it (throw away the date stamp)
    int space = s->find_first_of("  ");
    int colon1 = s->find_first_of(":");
    int colon2 = s->find_last_of(":");

    // find the two colons used in the time stamp, and the tab after the time stamp
    double hour = stod (s->substr(0, colon1), nullptr);
    double mins = stod (s->substr(colon1 + 1, colon2), nullptr);
    double secs = stod (s->substr(colon2 + 1, s->length()), nullptr);
    double total_secs = hour*3600 + mins*60 + secs;

    // check if the start time as already been recorded
    if (!(*start_set)) {
        *start_time = total_secs;
        *start_set = true;
    }

    // replce the time stamp with the seconds value, and replace the tab with a comma
    *s = to_string(total_secs - *start_time);
}

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

            string s;

            bool start_set = false;
            double start_time = 0;
            int i = 0;

            while (getline(infile, s)) {
                if (i == 0) { // replace tabs in the first line with commas
                    while (s.find("\t") != -1) {
                        s.replace(s.find("\t"), 1, ",");
                    }
                    i++;
                    outfile << s << "\n";
                } else { // if it is not the first line, convert the data

                    int tab = s.find_first_of("\t");                 // find the first tab
                    int space = s.find_first_of("  ");               // find the first double space
                    string time = s.substr(space + 1, tab);          // information between the tab and the space is the time stamp
                    convert_time(&time, &start_set, &start_time);    // convert the time stamp into seconds since the beginning
                    s.replace(0, (s.substr(0, tab).length()), time); // replace the date stamp and the time stamp with the time in seconds
                    tab = s.find_first_of("\t");                     // find the next tab
                    s.replace(tab, 1, ",");                          // replace the tab with a comma

                    // check if there are more tabs in this line, and convert the data if there is
                    if (s.find_first_of("\t") != -1) {

                        tab = s.find_first_of("\t");
                        s.replace(tab, 1, ",");
                        int comma = s.find_last_of(",");
                        tab = s.find_first_of("\t");
                        space = s.find_first_of("  ");
                        time = s.substr(space + 1, tab);
                        convert_time(&time, &start_set, &start_time);
                        s.replace(comma + 1, space - comma + 1, "");
                        tab = s.find_first_of("\t");
                        s.replace(tab, 1, ",");
                        tab = s.find_first_of("\t");
                        int last_comma = s.find_last_of(",");
                        s.replace(comma + 1, last_comma - comma - 1, time);
                    }

                    outfile << s << "\n";
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

