#include <gtest/gtest.h>
#include<RecognizerAppFactory.h>
#include <memory>
#include <tuple>

class IntentRecognizerFixtureTests : public ::testing::TestWithParam<std::tuple<std::string, Intent>> {
};

class IntentRecognizerFixtureTests_Negative : public ::testing::TestWithParam<std::tuple<std::string, Intent>> {
};

//Positive tests
INSTANTIATE_TEST_CASE_P(
        PositiveTests,
        IntentRecognizerFixtureTests,
        ::testing::Values(
                //Positive tests for Get Weather
                std::make_tuple("What is the weather like today?", Intent::kGet_Weather),
                std::make_tuple("WHAT IS THE WEATHER LIKE TODAY?", Intent::kGet_Weather),
                std::make_tuple("WHAT IS THE WEATHER LIKE TomoRRow?", Intent::kGet_Weather),
                std::make_tuple("WHAT's the Weather like in two days?", Intent::kGet_Weather),

                //Positive tests for Get Weather City
                std::make_tuple("What is the weather like in Paris today?", Intent::kGet_Weather_City),
                std::make_tuple("What's the weather like in Munich tomorrow?", Intent::kGet_Weather_City),
                std::make_tuple("What's THE weather LIKE in Stuttgart tomorrow?", Intent::kGet_Weather_City),
                std::make_tuple("What's THE weather LIKE in Berlin in four days?", Intent::kGet_Weather_City),

                //Positive tests for Check Calender
                std::make_tuple("Am I free at 13:00 PM tomorrow?", Intent::kCheck_Calender),
                std::make_tuple("Am I free at 14:00 PM today?", Intent::kCheck_Calender),
                std::make_tuple("Am I free at 17 PM tomorrow?", Intent::kCheck_Calender),
                std::make_tuple("AM I FREE at 02:00 AM tomorrow?", Intent::kCheck_Calender),

                //Positive tests for Get Fact
                std::make_tuple("interesting fact", Intent::kGet_Fact),
                std::make_tuple("Tell me an interesting fact.", Intent::kGet_Fact),
                std::make_tuple("tell me one interesting fact about berlin?", Intent::kGet_Fact)
        ));

TEST_P(IntentRecognizerFixtureTests, PositiveTests) {
    std::unique_ptr<IRecognizer> r;
    r = RecognizerFactory::createBasicIntent();
    auto input = std::get<0>(GetParam());
    auto expected = std::get<1>(GetParam());
    EXPECT_EQ(r->getIntent(input), expected);
}

//Negative tests
INSTANTIATE_TEST_CASE_P(
        NegativeTests,
        IntentRecognizerFixtureTests_Negative,
        ::testing::Values(
                //Negative tests for Get Weather
                std::make_tuple("What was the weather like yesterday?", Intent::kUnknown),
                std::make_tuple("WHATs IS THE WEATHER LIKE TODAY?", Intent::kUnknown),
                std::make_tuple("WHAT's THE WEATHER TomoRRow?", Intent::kUnknown),
                std::make_tuple("WHAT's the Weather like in two days from now?", Intent::kUnknown),

                //Negative tests for Get Weather City
                std::make_tuple("What is the weather like in Germany today?", Intent::kUnknown),
                std::make_tuple("What's the weather like in USA tomorrow?", Intent::kUnknown),
                std::make_tuple("What's is THE weather LIKE in Stuttgart tomorrow?", Intent::kUnknown),
                std::make_tuple("What's THE weather LIKE in Berlin in nine days?", Intent::kUnknown),

                //Negative tests for Check Calender
                std::make_tuple("Am I free at 13:00 PM in seven days?", Intent::kUnknown),
                std::make_tuple("My schedule at 14:00 PM today?", Intent::kUnknown),
                std::make_tuple("Was I free at 17 PM yesterday?", Intent::kUnknown),
                std::make_tuple("AM I FREE at 02:00 AM day after tomorrow?", Intent::kUnknown),

                //Negative tests for Get Fact
                std::make_tuple("facts", Intent::kUnknown),
                std::make_tuple("Tell me facts.", Intent::kUnknown)
        ));

TEST_P(IntentRecognizerFixtureTests_Negative, NegativeTests) {
    std::unique_ptr<IRecognizer> r;
    r = RecognizerFactory::createBasicIntent();
    auto input = std::get<0>(GetParam());
    auto expected = std::get<1>(GetParam());
    EXPECT_EQ(r->getIntent(input), expected);
}
