using Microsoft.VisualBasic.FileIO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArgeoSync.Helper
{
    public static class Importer
    {
        public static void ImportCSVToDataBase(
            out List<string> columns,
            out Dictionary<string, List<string>> values,
            string fileName)
        {
            using (TextFieldParser parser = new TextFieldParser(fileName))
            {
                parser.TextFieldType = FieldType.Delimited;
                parser.SetDelimiters(",");

                columns = new List<string>();
                values  = new Dictionary<string, List<string>>();

                //Process row
                string[] fields = parser.ReadFields();
                foreach (string field in fields)
                {
                    columns.Add(field);
                    List<string> vals = new List<string>();
                    values.Add(field, vals);
                }

                while (!parser.EndOfData)
                {
                    //Process row
                    fields = parser.ReadFields();
                    int index = 0;
                    foreach (string field in fields)
                    {
                        List<string> vals = values[columns[index]];
                        vals.Add(field);
                        index++;
                    }
                }
            }
        }

    }
}
