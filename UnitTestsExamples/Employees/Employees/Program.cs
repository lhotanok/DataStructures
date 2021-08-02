using System;
using System.Collections.Generic;

[assembly: System.Runtime.CompilerServices.InternalsVisibleTo("Employees_UnitTests")]

namespace EmployeeDatabase
{
    interface IEmployees
    {
        int Add(string name, int salary); //returns ID
        List<int> GetAll(); //returns list of IDs
        string GetName(int id);
        int GetSalary(int id);
        void ChangeSalary(int id, int newSalary);
    }
    interface IStatistics
    {
        int ComputeAverageSalary();
        int GetMinSalary();
        void PrintSalariesByName();
    }
    class Employee
    {
        public int Id { get; }
        public string Name { get; }
        public int Salary { get; set; }
        public Employee (int id, string name, int salary)
        {
            this.Id = id;
            this.Name = name;
            this.Salary = salary;
        }
    }
    class Employees : IEmployees
    {
        private Dictionary<int, Employee> employees; //keys are IDs
        private int freeId;
        public Employees()
        {
            this.employees = new Dictionary<int, Employee>();
            this.freeId = 0;
        }
        public int Add(string name, int salary)
        {
            Employee employee = new Employee(freeId, name, salary);
            employees.Add(employee.Id, employee);
            freeId++;
            return employee.Id;
        }
        public List<int> GetAll()
        {
            List<int> IDs = new List<int>();
            foreach (var employee in employees)
            {
                IDs.Add(employee.Value.Id);
            }
            return IDs;
        }
        public string GetName(int id)
        {
            Employee employee = employees[id];
            return employee.Name;
        }
        public int GetSalary(int id)
        {
            Employee employee = employees[id];
            return employee.Salary;
        }
        public void ChangeSalary(int id, int newSalary)
        {
            Employee employee = employees[id];
            employee.Salary = newSalary;
        }
    }
    class Statistics : IStatistics
    {
        private IEmployees employees;
        internal string consoleOutput = "";
        public Statistics (IEmployees em)
        {
            this.employees = em;
        }
        public int ComputeAverageSalary()
        {
            var IDs = employees.GetAll();
            int numberOfEmployees = IDs.Count;
            long averageSalary = -1;
            if (numberOfEmployees > 0)
            {
                long totalSalary = 0;
                foreach (var id in IDs)
                {
                    int salary = employees.GetSalary(id);
                    totalSalary += salary;
                }
                averageSalary = totalSalary / numberOfEmployees;
            }
            return (int)averageSalary; //returns -1 if no employee in database
        }
        public int GetMinSalary()
        {
            int minSalary = -1;
            var IDs = employees.GetAll();
            if (IDs.Count > 0)
            {
                minSalary = employees.GetSalary(IDs[0]);
                foreach (var id in IDs)
                {
                    int salary = employees.GetSalary(id);
                    if (salary < minSalary)
                    {
                        minSalary = salary;
                    }
                }
            }
            return minSalary; //returns -1 if no employee in database
        }
        public void PrintSalariesByName()
        {
            var employeeSalaries = GetSalariesSortedByEmployeeNames();
            foreach (var employee in employeeSalaries)
            {
                string name = employee.Key;
                int salary = employee.Value;
                consoleOutput += String.Format("<{0}, {1}>\n", name, salary);
                //Console.Out.WriteLine(String.Format("<{0}, {1}>", name, salary));
            }
            Console.Write(consoleOutput);
        }
        SortedDictionary<string, int> GetSalariesSortedByEmployeeNames ()
        {
            var salaries = new SortedDictionary<string, int>();
            var IDs = employees.GetAll();
            foreach (var id in IDs)
            {
                string name = employees.GetName(id);
                int salary = employees.GetSalary(id);
                salaries.Add(name, salary);
            }
            return salaries;
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            IEmployees employees = new Employees();
            employees.Add("Marketa Konecna", 28_000);
            employees.Add("Daniel Svetly", 41_400);
            employees.Add("Eliska Vesela", 30_900);
            IStatistics statistics = new Statistics(employees);
            statistics.PrintSalariesByName();
        }
    }
}