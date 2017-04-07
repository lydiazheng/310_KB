//
//  main.cpp
//  cmpt310-A3
//
//  Created by Lynn on 2016-11-14.
//  Copyright © 2016 Lydia. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool BackwardChaining(vector<string> KB, vector<string> body, vector<string> &solution_output);
void output(vector<string> solution_output, vector<string> KB,vector<string> body);

int main(){
    vector<string> rules; //set an array for each line in the database
    vector<string> KB; //seth the knowledge base
    
    //input file
    string line;
    int line_sum = 0;
    
    ifstream myfile ("data.txt");
    if (myfile.is_open()){
        while (getline(myfile, line, '\n')){
            rules.push_back(line);
            line_sum++;
        }
    }
    else{
        cout << "Fail to open the file" << endl;
    }
    myfile.close();

    //test for the input file
    for (int i = 0; i < line_sum ; i++){
        rules[i].erase(remove(rules[i].begin(), rules[i].end(), ' '), rules[i].end());
    }
    
    //obtain the query for the whole data
    string query;
    query = rules[0];
    
    //build the knowledge base
    for (int i = 0; i < (line_sum-1); i++){
        KB.push_back(rules[i+1]);
    }

    vector<string> body; // build an array for the character which needs to determine whether it is ture or false
    body.push_back(query); // initialize the body with the query
    
    //start the backward chaining
    vector<string> solution_output;
    bool solution;
    solution = BackwardChaining(KB, body, solution_output);
    
    //print out the solution
    if(solution == 1){
        cout << "The final output should be ture"<<endl<<endl;
        output(solution_output, KB, body);
    }
    else
        cout << "The final output is false, the KB can not prove the query." << endl;
    
    return 0;
}


//declare the method that runs the backward chaining algorithm
bool BackwardChaining(vector<string> KB,vector<string> body, vector<string> &solution_output){
    int count = 0; // count the character which is useless for the whole rule
    vector<string> body_new;
    
    //if the body array is empty, the recursion will stop
    if(body.size() == 0)
        return true;

    //if the body array is not empty, then use the backwardchaining method
    string KB_line, head, KB_char;
    for(int i = 0; i < KB.size(); i++){
        KB_line = KB[i];
        head = KB_line[0];// head is the first character in each KB line
        
        if(body[0] == head){
            solution_output.push_back(KB[i]);

            body.erase(remove(body.begin(), body.end(), head), body.end());
            
            for(int j = 1; j < KB[i].size(); j++){
                KB_char = KB_line[j];
                body_new.push_back(KB_char); //add the characters except the first one to the back of body array
            }
            
            //if the character is useless for the rules then delete it
            if (body_new.size() != 0){
                count++;
                if(count > 1){
                    body_new.erase(remove(body_new.begin(), body_new.end(), body_new[0]), body_new.end());
                    solution_output.erase(remove(solution_output.begin(), solution_output.end(), solution_output[i-1]), solution_output.end());
                }
            }
            
            for(int k = 0; k < body.size(); k++){
                body_new.push_back(body[k]);
            }
            if(BackwardChaining(KB, body_new, solution_output)== true){
                return true;                      //recursion
            }
        }
    }
    
    return false;

}


// output the soluition
// It is not important for the whole project = =just output
void output(vector<string> solution_output, vector<string> KB,vector<string> body){
    string solution;
    string query, character;
    int count = 0;
    vector<string> character_arr01;// character array for the useful characters
    query = body[0];
    
    //output the solution sentence
    cout << "The query '" << query << "' is true because: " << endl<< endl;
    
    for(int i = int(solution_output.size()-1); i >= 0; i--){
        if(solution_output[i].size() == 1)
            cout << solution_output[i] << " is ture" << endl;
        else if(solution_output[i].size() == 2){
            solution = solution_output[i];
            cout << solution[1] << " can implies " << solution[0] << endl;
        }
        else if (solution_output[i].size() >= 3){
            solution = solution_output[i];
            cout << solution[1] << " and "  << solution[2] << " can implies " << solution[0] << endl;
        }
        else{
            solution = solution_output[i];
            for (int j = 1; j < (solution.size()-1); j++){
                cout << solution[i] << " ,";
            }
            cout << " and " << solution[solution.size()-1] << " can implies " << solution[0] << endl;
        }
    }
    
    cout << "So that " << query << " is true." << endl << endl;
    
    //output the truth table for the solution
    for(int i = 0; i < solution_output.size(); i++){
        solution = solution_output[i];
        for(int j = 0; j < solution.size(); j++){
            character = solution[j];

            if(character_arr01.size() == 0){
                character_arr01.push_back(character);
                count++;
            }
            else{
                count = 1;
                for (int k = 0; k < character_arr01.size(); k++){
                    if(character != character_arr01[k])
                        count++;
                }
                if(count == character_arr01.size()+1){
                    character_arr01.push_back(character);
                }
            }
        }
    }
    cout << "T means only these rules are ture that the query '" << query << "' will be true"<<endl;
    cout << "The cahracter in the KB but abcent in the truth table means they are useless for proving the query." << endl << endl;
    
    cout << "---------------------------------------------" << endl;
    cout << "----------------Truth Tbale------------------" << endl;
    cout << "---------------------------------------------" << endl;
    for(int i = 0; i < character_arr01.size(); i++){
        cout << character_arr01[i] << "  ";
    }
    cout << "   ";
    for(int i = 0; i < solution_output.size(); i++){
        cout << solution_output[i] << "  ";
    }
    cout << endl;
    for(int i = 0; i < character_arr01.size(); i++){
        cout << "T  ";
    }
    cout << "   ";
    for(int i = 0; i < solution_output.size(); i++){
        cout << "T   ";
    }
    cout << endl<< endl;
}



