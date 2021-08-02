using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using EmployeeDatabase;

namespace Employees_UnitTests
{
    [TestClass]
    public class StatisticsTests
    {
        [TestMethod]
        public void ComputeAverageSalary_CalculateSalaryFromNonEmptyDatabase()
        {
            //Arrange
            IEmployees employees = new Employees();
            const int databaseSize = 2;
            const string name1 = "Karolina Nova";
            const int salary1 = 35_000;
            const string name2 = "Adam Pilar";
            const int salary2 = 32_500;
            int expectedAverageSalary = (salary1 + salary2) / databaseSize;
            employees.Add(name1, salary1);
            employees.Add(name2, salary2);
            IStatistics statistics = new Statistics(employees);

            //Act
            int averageSalary = statistics.ComputeAverageSalary();

            //Assert
            Assert.AreEqual(expectedAverageSalary, averageSalary);
        }
        [TestMethod]
        public void ComputeAverageSalary_CalculateSalaryFromEmptyDatabase()
        {
            //Arrange
            const int emptyDatabaseReturnValue = -1;
            IEmployees employees = new Employees();
            IStatistics statistics = new Statistics(employees);

            //Act
            int averageSalary = statistics.ComputeAverageSalary();

            //Assert
            Assert.AreEqual(emptyDatabaseReturnValue, averageSalary);
        }
        [TestMethod]
        public void GetMinSalary_GetSalaryFromNonEmptyDatabase()
        {
            //Arrange
            IEmployees employees = new Employees();
            const string name1 = "Karolina Nova";
            const int salary1 = 35_000;
            const string name2 = "Adam Pilar";
            const int salary2 = 32_500;
            employees.Add(name1, salary1);
            employees.Add(name2, salary2);
            IStatistics statistics = new Statistics(employees);

            //Act
            int minSalary = statistics.GetMinSalary();

            //Assert
            Assert.AreEqual(Math.Min(salary1, salary2), minSalary);
        }
        [TestMethod]
        public void GetMinSalary_GetSalaryFromEmptyDatabase()
        {
            //Arrange
            const int emptyDatabaseReturnValue = -1;
            IEmployees employees = new Employees();
            IStatistics statistics = new Statistics(employees);

            //Act
            int minSalary = statistics.GetMinSalary();

            //Assert
            Assert.AreEqual(emptyDatabaseReturnValue, minSalary);
        }
        [TestMethod]
        public void PrintSalariesByName_PrintWithNamesNotSortedOriginally()
        {
            //Arrange
            IEmployees employees = new Employees();
            const string name1 = "Karolina Nova";
            const int salary1 = 35_000;
            const string name2 = "Adam Pilar";
            const int salary2 = 32_500;
            employees.Add(name1, salary1);
            employees.Add(name2, salary2);
            IStatistics statistics = new Statistics(employees);
            string expectedOutput = String.Format("<{0}, {1}>\n", name2, salary2) + String.Format("<{0}, {1}>\n", name1, salary1);

            //Act
            statistics.PrintSalariesByName();

            //Assert
            Assert.AreEqual(expectedOutput, ((Statistics)statistics).consoleOutput);
        }
    }
}
