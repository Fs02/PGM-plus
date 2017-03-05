#include <cassert>
#include <string>
#include <pgm/pgm.h>

void test_variable()
{
    // with unspecified states
    pgm::Variable weather("weather");
    assert(weather("rainy") == 0);
    assert(weather("cloudy") == 1);
    assert(weather("sunny") == 2);
    assert(weather(0) == "rainy");
    assert(weather(1) == "cloudy");
    assert(weather(2) == "sunny");

    // with specified states
    pgm::Variable student("student", {"yes", "no"});
    assert(student("yes") == 0);
    assert(student("no") == 1);
    assert(student(0) == "yes");
    assert(student(1) == "no");
}

void test_dataset()
{
    pgm::Dataset dataset;
    dataset.push({{"outlook", "sunny"}, {"temperature", "hot"}, {"play", "yes"}}); 
    dataset.push({{"outlook", "rain"}, {"temperature", "cool"}, {"play", "no"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "cool"}, {"play", "yes"}});
    dataset.push({{"outlook", "sunny"}, {"temperature", "mild"}, {"play", "yes"}});
    dataset.push({{"outlook", "rain"}, {"temperature", "hot"}, {"play", "no"}});

    int a = dataset.size();
    assert(dataset.size() == 5);
    assert(dataset.names().size() == 3);
    assert(dataset.variables().size() == 3);
    assert(dataset.get("outlook", 0) == "sunny");
    assert(dataset.get("play", 4) == "no");
    assert(dataset.raw("temperature", 3) == 2);
}

int main()
{
    test_variable();
    test_dataset();
    return 0;
}