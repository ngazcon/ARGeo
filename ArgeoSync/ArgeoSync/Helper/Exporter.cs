using Microsoft.VisualBasic.FileIO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArgeoSync.Helper
{
    public static class Exporter
    {
        public static void ExportToCSVFile(
            List<string> columns,
            List<List<string>> values,
            string fileName)
        {
            using (var writer = new StreamWriter(fileName))
            {
                string line = "";
                foreach (string column in columns.Take(columns.Count - 1))
                {
                    line += column + ",";
                }
                line += columns.Last();
                writer.WriteLine(line);

                foreach (List<string> valuesInLine in values)
                {
                    if (valuesInLine.Count == 0)
                    {
                        continue;
                    }

                    line = "";
                    foreach (string value in valuesInLine.Take(valuesInLine.Count - 1))
                    {
                        line += value + ",";
                    }
                    line += valuesInLine.Last();
                    writer.WriteLine(line);
                }
                writer.Close();
            }
        }
    }
}
