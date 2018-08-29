using System;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Data;

namespace ArgeoSync.Converter
{
    [ValueConversion(typeof(Enum), typeof(bool))]
    public class IsEqualConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return parameter == value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
