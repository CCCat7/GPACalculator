#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <regex>
#include <sstream>
#include <cstdlib>

struct scoreLine {
    size_t score;
    double credit;
};

struct ruleLine {
    size_t lowerBound;
    size_t upperBound;
    double point;
};

class GPACalculator
{
private:
    std::string scoreFileName;
    std::string ruleFileName;

    std::vector<std::pair<std::string, std::string>> score;
    std::vector<std::pair<std::string, std::string>> rule;
    std::vector<scoreLine> parsedScore;
    std::vector<ruleLine> parsedRule;

    void ReadFile();
    void REAndDataProcess();
    void Calculate();
    
public:
    GPACalculator(const char *, const char *);
    ~GPACalculator();
    void Run();
};

GPACalculator::GPACalculator(const char *scoreFileName, const char *ruleFileName)
{
    this->scoreFileName = scoreFileName;
    this->ruleFileName = ruleFileName;
    ReadFile();
}

GPACalculator::~GPACalculator() {}

void GPACalculator::ReadFile()
{
    std::ifstream scoreIn, ruleIn;
    scoreIn.open(scoreFileName);
    if(!scoreIn.is_open()) {
        perror("Score input error");
        exit(0);
    }

    while(!scoreIn.eof()) {
        std::pair<std::string, std::string> tempLine;
        scoreIn >> tempLine.first >> tempLine.second;
        score.push_back(tempLine);
    }

    ruleIn.open(ruleFileName);
    if(!scoreIn.is_open()) {
        perror("Rule input error");
        exit(0);
    }

    while(!ruleIn.eof()) {
        std::pair<std::string, std::string> tempLine;
        ruleIn >> tempLine.first >> tempLine.second;
        rule.push_back(tempLine);
    }

    scoreIn.close();
    ruleIn.close();
}

void GPACalculator::REAndDataProcess()
{
    for(auto elem: score) {
        scoreLine tempLine;
        std::stringstream tempss;

        tempss.str(elem.first);
        tempss >> tempLine.score;
        
        tempLine.credit = atof(elem.second.c_str());

        parsedScore.push_back(tempLine);
    }


    std::string pattern = "([0-9]{1,3}*)(~|-|<|>*)([0-9]{1,3}*)";
    std::regex express(pattern);
    std::cout.setf(std::ios_base::boolalpha);
    
    std::match_results<std::string::iterator> ruleResults;
    for(auto elem: rule) {
        ruleLine tempLine;

        /*parse first*/
        if(std::regex_match(elem.first.begin(), elem.first.end(), ruleResults, express)) {
            std::string parts[3];
            std::match_results<std::string::iterator>::const_iterator iter = ruleResults.begin();
            iter++;

            size_t i = 0;
            for (; iter != ruleResults.end(); iter++) {
                parts[i] = iter->str();
                ++i;
            }

            /* <x, >x */
            if(parts[0].size() == 0 && parts[1].size() != 0 && parts[2].size() != 0) {
                if(parts[1] == ">") {
                    tempLine.upperBound = 100;
                    std::stringstream tempss;
                    tempss.str(parts[2]);
                    tempss >> tempLine.lowerBound;
                    ++tempLine.lowerBound;
                }
                else {
                    tempLine.lowerBound = 0;
                    std::stringstream tempss;
                    tempss.str(parts[2]);
                    tempss >> tempLine.upperBound;
                    --tempLine.upperBound;
                }
            }
            else if(parts[0].size() != 0 && parts[1].size() == 0 && parts[2].size() == 0) {
                std::stringstream tempss;

                tempss.str(parts[0]);
                tempss >> tempLine.lowerBound;
                tempLine.upperBound = tempLine.lowerBound;
            }
            else {
                std::stringstream tempss;
                tempss.str(parts[0]);
                tempss >> tempLine.upperBound;
                tempss.clear();
                tempss.str("");
                tempss.str(parts[2]);
                tempss >> tempLine.lowerBound;
            }
        }

        tempLine.point = atof(elem.second.c_str());
        parsedRule.push_back(tempLine);
    }
}

void GPACalculator::Calculate()
{
    /*Calculate*/
    double creditMulPoint = 0.0;
    double creditTotal = 0.0;

    for(auto elem: parsedScore) {
        for(auto range: parsedRule) {
            if((elem.score >= range.lowerBound) && (elem.score <= range.upperBound)) {
                creditTotal += elem.credit;
                creditMulPoint += range.point * elem.credit;
                break;
            }
            else 
                continue;
        }
    }

    double GPA = creditMulPoint / creditTotal;
    std::cout << "GPA: " << GPA << std::endl;
}

void GPACalculator::Run()
{
    REAndDataProcess();
    Calculate();
}
