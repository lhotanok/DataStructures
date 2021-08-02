using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace SortedDictionary_UnitTests
{
    [TestClass]
    public class SortedDictionaryTests
    {
        [TestMethod]
        public void Add_NewKeyValuePairWithNonExistingKey()
        {
            //Arrange
            int key = 1;
            int value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>();

            //Act
            dict.Add(key, value);

            //Assert
            Assert.AreEqual(value, dict[key]);
        }
        [TestMethod]
        public void Add_NewKeyValuePairWithExistingKey()
        {
            //Arrange
            int key = 1;
            int value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            Action keyValuePairAddition = new Action(AddKeyValuePair);
            void AddKeyValuePair()
            {
                dict.Add(key, value);
            }

            //Assert
            Assert.ThrowsException<ArgumentException>(keyValuePairAddition);
        }
        [TestMethod]
        public void Remove_KeyValuePairPresentInDictionary()
        {
            //Arrange
            const int emptyDictSize = 0;
            int key = 1;
            int value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            dict.Remove(key);

            //Assert
            Assert.AreEqual(emptyDictSize, dict.Count);
        }
        [TestMethod]
        public void Remove_NonExistingKeyValuePair()
        {
            //Arrange
            int key1 = 1, value1 = 10;
            int key2 = 2, value2 = 5;
            int keyToRemove = 3;
            SortedDictionary<int, int> originalDict = new SortedDictionary<int, int>() { { key1, value1 }, { key2, value2 } };
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key1, value1 }, { key2, value2 } };

            //Act
            dict.Remove(keyToRemove);

            //Assert
            foreach (var pair in originalDict)
            {
                Assert.AreEqual(pair.Value, dict[pair.Key]);
            }
        }
        [TestMethod]
        public void Clear_RemoveAllElementsAndCheckForEmptiness()
        {
            //Arrange
            const int emptyDictSize = 0;
            int key1 = 1, value1 = 10;
            int key2 = 2, value2 = 5;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key1, value1 }, { key2, value2 } };

            //Act
            dict.Clear();

            //Assert
            Assert.AreEqual(emptyDictSize, dict.Count);
        }
        [TestMethod]
        public void TryGetValue_ValueAssociatedWithExistingKey()
        {
            //Arrange
            int key = 1;
            int value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            dict.TryGetValue(key, out int requestedValue);

            //Assert
            Assert.AreEqual(value, requestedValue);
        }
        [TestMethod]
        public void TryGetValue_ValueNotAssociatedWithAnyExistingKey()
        {
            //Arrange
            int key = 1, value = 10;
            int nonExistingKey = 2;
            int defaultIntValue = 0;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            dict.TryGetValue(nonExistingKey, out int requestedValue);

            //Assert
            Assert.AreEqual(defaultIntValue, requestedValue);
        }
        [TestMethod]
        public void ContainsKey_CheckPresenceOfExistingKey()
        {
            //Arrange
            const bool keyExists = true;
            int key = 1, value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            bool keyInDict = dict.ContainsKey(key);

            //Assert
            Assert.AreEqual(keyExists, keyInDict);
        }
        [TestMethod]
        public void ContainsKey_CheckPresenceOfNonExistingKey()
        {
            //Arrange
            const bool keyExists = true;
            int key = 1, value = 10;
            int nonExistingKey = 2;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };

            //Act
            bool keyInDict = dict.ContainsKey(nonExistingKey);

            //Assert
            Assert.AreNotEqual(keyExists, keyInDict);
        }
        [TestMethod]
        public void GetEnumerator_CheckIterationOverSortedKeys()
        {
            //Arrange
            int key1 = 3, value1 = 10;
            int key2 = 1, value2 = 5;
            int key3 = 2, value3 = 6;
            int[] sortedKeys = new int[3] { key2, key3, key1 };
            int[] keysByDictOrder = new int[3];
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key1, value1 }, { key2, value2 }, { key3, value3 } };

            //Act
            int index = 0;
            foreach (var pair in dict)
            {
                keysByDictOrder[index] = pair.Key;
                index++;
            }

            //Assert
            for (int i = 0; i < sortedKeys.Length; i++)
            {
                Assert.AreEqual(sortedKeys[i], keysByDictOrder[i]);
            }
        }
        [TestMethod]
        public void Remove_KeyPresentInDictionary_DemonstrateFailedTest()
        {
            //Arrange
            int key = 1, value = 10;
            SortedDictionary<int, int> dict = new SortedDictionary<int, int>() { { key, value } };
            int originalCount = dict.Count;

            //Act
            dict.Remove(key);

            //Assert
            Assert.AreEqual(originalCount, dict.Count);
        }
    }
}