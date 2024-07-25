/*tests.c*/

//
// Google test cases for our set class.
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <set>  // for comparing answers

using std::string;
using std::vector;
using std::pair;

#include "set.h"
#include "gtest/gtest.h"


TEST(myset, empty_set)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);
}

TEST(myset, set_with_one)
{
  set<int> S;

  ASSERT_EQ(S.size(), 0);

  S.insert(123);

  ASSERT_EQ(S.size(), 1);

  ASSERT_TRUE(S.contains(123));
  ASSERT_TRUE(S[123]);

  ASSERT_FALSE(S.contains(100));
  ASSERT_FALSE(S[100]);
  ASSERT_FALSE(S.contains(200));
  ASSERT_FALSE(S[200]);
}

TEST(myset, set_with_four_strings)
{
  set<string> S;

  ASSERT_EQ(S.size(), 0);

  S.insert("banana");
  S.insert("apple");
  S.insert("chocolate");
  S.insert("pear");

  ASSERT_EQ(S.size(), 4);

  ASSERT_TRUE(S.contains("pear"));
  ASSERT_TRUE(S["banana"]);
  ASSERT_TRUE(S.contains("chocolate"));
  ASSERT_TRUE(S["apple"]);

  ASSERT_FALSE(S.contains("Apple"));
  ASSERT_FALSE(S["carmel"]);
  ASSERT_FALSE(S.contains("appl"));
  ASSERT_FALSE(S["chocolatee"]);
}

class Movie
{
public:
  string Title;
  int    ID;
  double Revenue;

  Movie(string title, int id, double revenue)
    : Title(title), ID(id), Revenue(revenue)
  { }

  bool operator<(const Movie& other)
  {
    if (this->Title < other.Title)
      return true;
    else
      return false;
  }
};

TEST(myset, set_with_movies)
{
  set<Movie> S;

  ASSERT_EQ(S.size(), 0);

  Movie Sleepless("Sleepless in Seattle", 123, 123456789.00);
  S.insert(Sleepless);

  Movie Matrix("The Matrix", 456, 400000000.00);
  S.insert(Matrix);

  Movie AnimalHouse("Animal House", 789, 1000000000.00);
  S.insert(AnimalHouse);

  ASSERT_EQ(S.size(), 3);

  vector<Movie> V = S.toVector();

  ASSERT_EQ(V[0].Title, "Animal House");
  ASSERT_EQ(V[1].Title, "Sleepless in Seattle");
  ASSERT_EQ(V[2].Title, "The Matrix");
}

TEST(myset, set_from_class_with_nine)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S[0]);
  ASSERT_FALSE(S.contains(2));
  ASSERT_FALSE(S[2]);
  ASSERT_FALSE(S.contains(4));
  ASSERT_FALSE(S[4]);
  ASSERT_FALSE(S.contains(29));
  ASSERT_FALSE(S[31]);
  ASSERT_FALSE(S.contains(40));
  ASSERT_FALSE(S[42]);
}

TEST(myset, set_no_duplicates)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  // try to insert them all again:
  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
    ASSERT_TRUE(S[x]);
  }
}

TEST(myset, toVector)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  vector<int> V2 = S.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, copy_empty)
{
  set<int> S1;

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), 0);

    vector<int> V2 = S2.toVector();

    ASSERT_EQ((int) V2.size(), 0);
  }
}

TEST(myset, copy_constructor)
{
  set<int> S1;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S1.insert(x);

  ASSERT_EQ(S1.size(), (int) V.size());

  {
    //
    // create a new scope, which will trigger destructor:
    //
    set<int> S2 = S1;  // this will call copy constructor:

    S1.insert(123);  // this should have no impact on S2:
    S1.insert(100);
    S1.insert(150);

    ASSERT_EQ(S2.size(), (int) V.size());

    vector<int> V2 = S2.toVector();

    ASSERT_EQ(V2.size(), V.size());

    std::sort(V.begin(), V.end());

    //
    // V and V2 should have the same elements in 
    // the same order:
    //
    auto iterV = V.begin();
    auto iterV2 = V2.begin();

    while (iterV != V.end()) {
      ASSERT_EQ(*iterV, *iterV2);

      iterV++;
      iterV2++;
    }

    S2.insert(1000);  // this should have no impact on S1:
    S2.insert(2000);
    S2.insert(3000);
    S2.insert(4000);
    S2.insert(5000);

    V.push_back(123);
    V.push_back(100);
    V.push_back(150);
  }

  //
  // the copy was just destroyed, the original set
  // should still be the same as it was earlier:
  //
  ASSERT_EQ(S1.size(), (int) V.size());

  vector<int> V2 = S1.toVector();

  ASSERT_EQ(V2.size(), V.size());

  std::sort(V.begin(), V.end());

  //
  // V and V2 should have the same elements in 
  // the same order:
  //
  auto iterV = V.begin();
  auto iterV2 = V2.begin();

  while (iterV != V.end()) {
    ASSERT_EQ(*iterV, *iterV2);

    iterV++;
    iterV2++;
  }
}

TEST(myset, find_empty)
{
  set<int> S;

  auto iter = S.find(22);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_one)
{
  set<int> S;

  S.insert(1234);

  auto iter = S.find(123);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1234);
  if (iter == S.end()) {  // this should not happen:
    ASSERT_TRUE(false); // fail:
  }

  ASSERT_EQ(*iter, 1234);

  iter = S.find(1235);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, find_with_set_from_class)
{
  set<int> S;

  vector<int> V = { 22, 11, 49, 3, 19, 35, 61, 30, 41 };

  for (auto x : V)
    S.insert(x);

  ASSERT_EQ(S.size(), (int) V.size());

  //
  // make sure we can find each of the values we inserted:
  //
  for (auto x : V) {
    auto iter = S.find(x);

    if (iter == S.end()) {  // this should not happen:
      ASSERT_TRUE(false); // fail:
    }

    ASSERT_EQ(*iter, x);
  }

  //
  // these searches should all fail:
  //
  auto iter = S.find(0);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(-1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(1);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(4);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(34);
  ASSERT_TRUE(iter == S.end());

  iter = S.find(36);
  ASSERT_TRUE(iter == S.end());
}

TEST(myset, stress_test)
{
  set<long long> S;
  std::set<long long> C;

  long long N = 1000000;

  //
  // setup random number generator so tree will
  // be relatively balanced given insertion of
  // random numbers:
  //
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> distrib(1, N * 100);  // inclusive

  vector<long long> V;  // collect a few values for searching:
  int count = 0;

  while (S.size() != N) {

    long long x = distrib(gen);

    S.insert(x);
    C.insert(x);

    count++;
    if (count == 1000) { // save every 1,000th value:

      V.push_back(x);
      count = 0;
    }
  }

  ASSERT_EQ(S.size(), N);

  for (auto x : V) {
    ASSERT_TRUE(S.contains(x));
  }

  ASSERT_FALSE(S.contains(0));
  ASSERT_FALSE(S.contains(-1));

  //
  // now let's compare our set to C++ set:
  //
  V.clear();
  V = S.toVector();

  ASSERT_EQ(V.size(), C.size());
  ASSERT_EQ(S.size(), (int) C.size());

  int i = 0;

  for (auto x : C) {
    ASSERT_EQ(V[i], x);
    i++;
  }
}


//
// toPair tests
//
TEST(myset, empty_set_toPair) 
{
  set<int> S;

  vector<pair<int, int>> V;
  V = S.toPairs(-1);
  ASSERT_EQ((int) V.size(), 0);
}

TEST(myset, set_with_one_toPair) 
{
  set<int> S;
  S.insert(123);

  vector<pair<int, int>> V;
  V = S.toPairs(-1);
  ASSERT_EQ((int) V.size(), 1);

  ASSERT_EQ(V.at(0), std::make_pair(123, -1));
}

TEST(myset, set_with_four_ints_toPair) 
{
  set<int> S;

  S.insert(123);
  S.insert(789);
  S.insert(456);
  S.insert(50);

  vector<pair<int, int>> V;
  V = S.toPairs(-1);
  ASSERT_EQ((int) V.size(), 4);

  ASSERT_EQ(V.at(0), std::make_pair(50, 123));
  ASSERT_EQ(V.at(1), std::make_pair(123, -1));
  ASSERT_EQ(V.at(2), std::make_pair(456, 789));
  ASSERT_EQ(V.at(3), std::make_pair(789, -1));
}

TEST(myset, set_with_ten_ints_toPair) 
{
  set<int> S;

  S.insert(30);
  S.insert(15);
  S.insert(50);
  S.insert(8);
  S.insert(25);
  S.insert(70);
  S.insert(20);
  S.insert(28);
  S.insert(60);
  S.insert(80);

  vector<pair<int, int>> V;
  V = S.toPairs(-1);
  ASSERT_EQ((int) V.size(), 10);

  ASSERT_EQ(V.at(0), std::make_pair(8, 15));
  ASSERT_EQ(V.at(1), std::make_pair(15, -1));
  ASSERT_EQ(V.at(2), std::make_pair(20, 25));
  ASSERT_EQ(V.at(3), std::make_pair(25, -1));
  ASSERT_EQ(V.at(4), std::make_pair(28, 30));
  ASSERT_EQ(V.at(5), std::make_pair(30, -1));
  ASSERT_EQ(V.at(6), std::make_pair(50, -1));
  ASSERT_EQ(V.at(7), std::make_pair(60, 70));
  ASSERT_EQ(V.at(8), std::make_pair(70, -1));
  ASSERT_EQ(V.at(9), std::make_pair(80, -1));
}

TEST(myset, set_with_four_strings_toPair) 
{
  set<string> S;

  S.insert("banana");
  S.insert("apple");
  S.insert("pear");
  S.insert("chocolate");


  vector<pair<string, string>> V;
  V = S.toPairs("null");
  ASSERT_EQ((int) V.size(), 4);

  ASSERT_EQ(V.at(0), std::make_pair((string) "apple", (string) "banana"));
  ASSERT_EQ(V.at(1), std::make_pair((string) "banana", (string) "null"));
  ASSERT_EQ(V.at(2), std::make_pair((string) "chocolate", (string) "pear"));
  ASSERT_EQ(V.at(3), std::make_pair((string) "pear", (string) "null"));
}

//
// foreach loop tests
//
TEST(myset, empty_set_foreach) 
{
  set<int> S;

  vector<int> exp = {}; // expected order
  vector<int> act; // actual order by copying from set

  for (int i : S) {
    act.push_back(i);
  }

  ASSERT_EQ(exp, act);
}

TEST(myset, set_with_one_foreach) 
{
  set<int> S;

  S.insert(123);

  vector<int> exp = {123};
  vector<int> act;

  for (int i : S) {
    act.push_back(i);
  }

  // check that each element in expected and actual set match up
  for (size_t i = 0; i < act.size(); i++) {
    ASSERT_EQ(act[i], exp[i]);
  }
}

TEST(myset, set_with_ten_ints_foreach) 
{
  set<int> S;

  S.insert(123);
  S.insert(456);
  S.insert(789);
  S.insert(10);
  S.insert(20);
  S.insert(384);
  S.insert(32);
  S.insert(230);
  S.insert(283);
  S.insert(1);

  vector<int> exp = {1, 10, 20, 32, 123, 230, 283, 384, 456, 789};
  vector<int> act;

  for (int i : S) {
    act.push_back(i);
  }


  for (size_t i = 0; i < act.size(); i++) {
    ASSERT_EQ(act[i], exp[i]);
  }
}

TEST(myset, set_with_four_strings_foreach) 
{
  set<string> S;

  S.insert("banana");
  S.insert("apple");
  S.insert("pear");
  S.insert("chocolate");

  vector<string> exp = {"apple", "banana", "chocolate", "pear"};
  vector<string> act;

  for (string s : S) {
    act.push_back(s);
  }

  // check that each element in expected and actual set match up
  for (size_t i = 0; i < act.size(); i++) {
    ASSERT_EQ(act[i], exp[i]);
  }
}

TEST(myset, stress_test_foreach) 
{
  set<long long> S;
  long long N = 1000000;
  std::set<long long> exp; // C++ set

  //
  // setup random number generator so tree will
  // be relatively balanced given insertion of
  // random numbers:
  //
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> distrib(1, N * 100);  // inclusive


  while (S.size() != N) {
    long long x = distrib(gen);
    S.insert(x);
    exp.insert(x);
  }

  vector<long long> act;
  for (long long i : S) {
    act.push_back(i);
  }

  // check that order copied from set equals C++'s set order
  int i = 0;
  for (long long x : exp) {
    ASSERT_EQ(act[i], x);
    i++;
  }
}