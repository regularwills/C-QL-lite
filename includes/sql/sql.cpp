#include "sql.h"

//CTORS
SQL::SQL(){
}


//SQL command line takes in the string. Runs it through the ptree to be initalized
// ptree with give me the table, the command, and the conditions
// run the conditions though the select input for string
// get me those rec numbers


Table SQL::command(const string input){
    //Conver string to c string
    char sql_command[input.size()];
    strcpy(sql_command, input.c_str());
 
    //Pass the SQL command to the parser and italzie the ptree variable
    Parser parser(sql_command);
    ptree = parser.parse_tree();

    
    //Check the ptree was initalized
    assert(!ptree.empty());

    //For debug
   //ptree.print_lookup();
    //cout << __LINE__ << endl;


    vectorstr from = ptree["table"];  if (!from.empty() && from[0] == ""){from.erase(from.begin());}
    vectorstr values = ptree["values"]; if (!values.empty() && values[0] == ""){values.erase(values.begin());}
    vectorstr fields = ptree["fields"];  if (!fields.empty() && fields[0] == ""){fields.erase(fields.begin());}
    vectorstr command = ptree["command"];  if (!command.empty() && command[0] == ""){command.erase(command.begin());}
    vectorstr conditions = ptree["conditions"];  if (!conditions.empty() && conditions[0] == ""){conditions.erase(conditions.begin());}


    // cout << "from: " << from << endl;
    //  cout << "values: " << values << endl;
    //  cout << "fields: " << fields << endl;
    //  cout << "command: " << command << endl;
    //  cout << "conditions: " << conditions << endl;


    //EVALUATING THE SQL "COMMAND"
    if (command.empty()){
        cout << "\n\n *** COMMAND IS EMPTY AND WAS NOT INITALIZED PROPRERLY*** \n\n";
    }
    //Pass the command from the SQL command line into the table
    else {
        //Command is "make/create"
        if (command[0] == "make" || command[1] == "create"){
            //Create a new binary file and txt file with apporpraite fields
            table = Table(from[0], fields);
        }
        //Command is "select"
        else if(command[0] == "select"){
            //If fields is a *, return everything from a specific table
            if (fields[0] == "*"){
                Table selected = table.select_all();
            }
            // Fields is NOT a "*" and there are certian fields to be met
            // run it through the infix select command
            else {
                Table selected = table.select(fields, conditions);
            }
        }
        //Command is "insert"
        else if (command[0] == "insert"){
            Table tbl(from[0]);
            tbl.insert_into(values);
        }
        //Command is not recognized as a valid SQL command
        else{
            cout << "INVALID SQL COMMAND: " << command[1] << endl;
            cout << "\n\nexiting....\n";
            exit(0);
        }
    }


    return table;
}

vectorlong SQL::select_recnos(){
    return table.select_recnos();
}


//Command takes int the string, go through if conditions and call that command
