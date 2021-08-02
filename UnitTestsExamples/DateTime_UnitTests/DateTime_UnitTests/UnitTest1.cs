using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace DateTime_UnitTests
{
    [TestClass]
    public class DateTimeTests
    {
        [TestMethod]
        public void DateTime_InicializeDateTimeWithTicksParameterPassed()
        {
            //Arrange
            long ticks = 2020; //within MinValue and MaxValue range

            //Act
            var dateTime = new DateTime(ticks);

            //Assert
            Assert.IsTrue(dateTime.Ticks == ticks);
        }
        [TestMethod]
        public void DateTime_TryInicializeDateTimeWithMaxTicksExceeded()
        {
            //Arrange
            long ticks = DateTime.MaxValue.Ticks + 1;

            //Act
            Action dateTimeInicialization = new Action(InicializeDateTime);
            void InicializeDateTime()
            {
                var dateTime = new DateTime(ticks);
            }

            //Assert
            Assert.ThrowsException<ArgumentOutOfRangeException>(dateTimeInicialization);
        }
        [TestMethod]
        public void ToString_ConvertDateTimeObjectToString()
        {
            //Arrange
            DateTime dateTime = new DateTime();
            string expectedDateFormat = "01.01.0001 0:00:00";

            //Act
            var date = dateTime.ToString();

            //Assert
            Assert.IsInstanceOfType(date, expectedDateFormat.GetType());
        }
        [TestMethod]
        public void TryParse_ConvertStringDateFormatToDateTime()
        {
            //Arrange
            string textDateTime = "Mon, 15 May 2009 20:10:57 GMT";
            DateTime dateTime = new DateTime();

            //Act
            DateTime.TryParse(textDateTime, out DateTime parsedDate);

            //Assert
            Assert.IsInstanceOfType(parsedDate, dateTime.GetType());
        }
        [TestMethod]
        public void TryParse_TryConvertInvalidDateFormatIntoDateTime()
        {
            //Arrange
            string textDateTime = "Lorem ipsum";

            //Act
            bool parseSucceeded = DateTime.TryParse(textDateTime, out DateTime parsedDate);

            //Assert
            Assert.IsFalse(parseSucceeded);
        }
    }
}
