#include "table.h"

//Default ctor
Table::Table(){
    _last_record_number = 0;
    _serial_number = 0;
}

//GTG
//Connects this table to an existing binary and txt file
Table::Table(const string &table_name){
    _name = table_name;
    _last_record_number = 0;
    _serial_number = 0;
    vector<string> field_names;

    fstream f;

    read_info((_name+"_fields.txt").c_str(), field_names);
    set_fields(field_names);
    //reindex();

    // set_fields(field_names);
    open_fileRW(f, (_name+".tbl").c_str());

    

    //rebuild index structure from binary file
    reindex();
    
    f.close();
}

//GTG
//Creates a binary table and various look up structures
Table::Table(const string &table_name, const vector<string>& field_names){
    //Create a new table that doesn't already exist
    _serial_number = 0;
    _last_record_number = 0;
    _name = table_name;

    fstream new_table_f;

    //Build member data structures to hold indices and
    set_fields(field_names);

    //create an empty binary file with same name as table
    open_fileW(new_table_f, (_name+".tbl").c_str());

    //write these fields to the fields.txt
    write_info((_name+"_fields.txt").c_str(), _field_names);

    new_table_f.close();
}

//GTG
//Open the binary file and print our each record in that file
 ostream& operator<<(ostream& outs, const Table& print_me){
    fstream file;
    FileRecord r;

    open_fileRW(file, (print_me._name+".tbl").c_str());

    outs << endl;
    outs << setw(25) << "record";
    for (int i = 0; i < print_me._field_names.size(); i++){
        outs << setw(25) << print_me._field_names[i];
    }
    outs << endl;
    
    int i = 0;
    long bytes = r.read(file,i);
    while (bytes>0){
        outs << setw(25) << i << setw(25) << r << endl;
        i++;
        bytes = r.read(file, i);
    }
    return outs;
}

//GTG
// Insert a new row of values into the table
void Table::insert_into(vector<string>& record_info){
    fstream f;


    //Write the record data into the binary .tbl file
    open_fileRW(f, (_name+".tbl").c_str());
    FileRecord r(record_info);

    //write the record to file, get that individual rec number
    _last_record_number = r.write(f);
    used_records.push_back(_last_record_number);

    //Update _indices structure
    for (int i = 0; i < record_info.size(); i++){
        string record_field = record_info[i];
        _indices[i][record_field] += _last_record_number;
       
    }


   
    f.close();
}

Table& Table::select(const vectorstr& fields, vectorstr& infix_condition){
    //Take the infix_conditions and run it through the shunting yard algo

    queue<Token *> postfix_condition;
    infix_to_postfix(infix_condition, postfix_condition);

    //Return the table from the postfix select
    return select(fields, postfix_condition);
}

Table& Table::select(const vectorstr& fields, queuetokens& postfix_queue){
    //used_records.clear();

    vectorstr regular_tokens;
    vectorstr relational_tokens;
    vectorstr logical_tokens;
    
    vector<vectorlong> records;
    vectorlong final_records;

    //Run through the postfix expression and sort tokesn according to their types
    for (int i = 0; !postfix_queue.empty(); i++){
        
        //Popping token from the top of queue and sorting to appropriate vectors
        Token* token = postfix_queue.front();
        postfix_queue.pop();

        //cout << token->token_str() << endl;

        if (token->type() == REG_TOKEN){
            regular_tokens.push_back(token->token_str());
        }
        if (token->type() == RELATIONAL){ //">, < , <=, >=, or ="
            relational_tokens.push_back(token->token_str());
        }
        if (token->type() == LOGICAL_OR || token->type() == LOGICAL_AND){
            logical_tokens.push_back(token->token_str());
        }

        // You have two regular tokens, most likely a field and condition, and one relational token to
        // make a select infix operation.. i.e. "lname = May, age > 20...."
        if (regular_tokens.size() == 2 && relational_tokens.size() == 1){
            //Store the records of the inifx expression
            records.push_back(get_record_nums(fields, regular_tokens[0], relational_tokens[0], regular_tokens[1]));
            
            //Clear the tokens and start over if necessary
            regular_tokens.clear();
            relational_tokens.clear();
        }
    
        //Checking for a logical "or"/"and" in the queue statement
        if (logical_tokens.size() == 1){
            //When we have an "and", we want the intersection of records and need to erase any duplicates
            if (token->token_str() == "and"){

                vectorlong intersection_records;
                intersection_records = intersection(records[0], records[1]);
                records.clear();

                //clear all records except for that intersection
                records.push_back(intersection_records);            
                logical_tokens.clear();
            }
            //Or doesn't really do anything but clear the logical list 
            if (token->token_str() == "or"){

                vectorlong combined_records;
                combined_records = records[0] + records[1];
                records.clear();

                records.push_back(combined_records);
                logical_tokens.clear();
            }
        }
    }
    //Accumulate all the records gathered by records and add them together
    for (int i = 0; i < records.size(); i++){
        final_records + records[i];
    }

    used_records = final_records;

    //The entire postfix queue has been evaluated and records contains all the records that pertain to 
    // each section of the statement
    //Reading and writing each of these record into a new table and returning that
    string new_name = _name + to_string(_serial_number++);
    Table temp_table(new_name, fields);
    
    fstream f;
    FileRecord r;
    open_fileRW(f, (_name+".tbl").c_str());

    vectorstr record_info;
    for (int i = 0; i < final_records.size(); i++){
        r.read(f, final_records[i]);
        for(int i = 0; i < fields.size(); i++){
            record_info.push_back(r._record[field_map[fields[i]]]);
        }
        temp_table.insert_into(record_info);
        record_info.clear();
    }
    f.close();
    cout << temp_table;
    return temp_table;
}

Table& Table::select(const vectorstr& fields, const string& field, 
                     const string& relational, const string& condition){
    
    used_records.clear();
    vectorlong record_field;

    //For the filerecord output order
    for (int i = 0; i < fields.size(); i++){
        record_field.push_back(field_map[fields[i]]);
    }
    cout << endl;

    //Which multimap based on the first field name
    int row = field_map[field];

    //Conditional statement for the different symbols(operators that will be passed through
    if (relational == "="){
        used_records = _indices[row][condition];
    }
    if (relational == "<"){
        mmap_sl::Iterator it = _indices[row].begin();
        for (; it != _indices[row].lower_bound(condition); it++){
                used_records + (*it).value_list;
                // cout << *it << endl;
        }
        used_records + (*it).value_list;
    }
    if (relational == ">"){
        mmap_sl::Iterator it = _indices[row].upper_bound(condition);
        it++;
        for (; it != _indices[row].end(); it++){
            used_records + (*it).value_list;
        }
    }
    if (relational == "<="){
        mmap_sl::Iterator it = _indices[row].begin();
        mmap_sl::Iterator end_it = _indices[row].upper_bound(condition);
        end_it++;

        for (; it != end_it; it++){
                used_records = used_records + (*it).value_list;
        }
    }
    if (relational == ">="){
        mmap_sl::Iterator it = _indices[row].upper_bound(condition);
        for (; it != _indices[row].end(); it++){
                used_records = used_records + (*it).value_list;
        }
    }
    
    string new_name = _name + to_string(_serial_number++);
    Table temp_table(new_name, fields);
    
    fstream f;
    FileRecord r;
    open_fileRW(f, (_name+".tbl").c_str());

    vectorstr record_info;
    //grab from 2d array from to
    for (int i = 0; i < used_records.size(); i++){
        r.read(f, used_records[i]);
        for(int i = 0; i < fields.size(); i++){
            record_info.push_back(r._record[field_map[fields[i]]]);
        }
        temp_table.insert_into(record_info);
        record_info.clear();
    }
    f.close();

    return temp_table;
}


//GTG
//Read and output all the files from a .tbl binary file
Table& Table::select_all(){
    fstream file;
    FileRecord r;

    open_fileRW(file, (_name+".tbl").c_str());

    cout << setw(25) << "record";
    for (int i = 0; i < _field_names.size(); i++){
        cout << setw(25) << _field_names[i];
    }
    cout << endl;
    for (int i = 0; i <= _last_record_number; i++){
        cout << setw(25) << i;                  // Record num
        r.read(file,i);                         // grab the record
        cout << r << endl;                      // output the used_records fields
    }
    return *this;
}

// Return a vector off record nums
vectorlong Table::select_recnos(){
    return used_records;
}

//GTG
void Table::set_fields(const vector<string>& field_names){
    //Member vector<string> is equal to argument
    _field_names = field_names;
    
    //Initalize field map with their keys and values
    for (int i = 0; i < _field_names.size(); i++){
        field_map.insert(_field_names[i], i);
    }

    //Pushing an index(mmap) into the indices vector
    for (int i = 0; i < _field_names.size(); i++){
        _indices.push_back(mmap_sl());
    }
}

//GTG
// Initalize indicies mmaps from an existing binary file
void Table::reindex(){
    fstream f;
    FileRecord r;
    _last_record_number = 0;

    //open the file for reading and writing.
    open_fileRW(f, (_name+".tbl").c_str());
    long i = 0;
    long bytes = r.read(f, i); //empty envelop to be filled by the FileRecord object
    
    //Read each record from bin file and insert those record and fields into the member indices vector
    while (bytes > 0){ //check till file size is 0
    
    //add each field to the mmaps
      for (int j = 0; j < _field_names.size(); j++){
        string record_field = r._record[j];
        _indices[j][record_field] += i;
      }
        i++;
        bytes = r.read(f, i);
    }
    _last_record_number = i;
    f.close();
}

//GTG
//Takes a simple infix statment field = this, field > num, field <= this, and returns the 
// record numbers associated with that
vectorlong Table::get_record_nums(const vectorstr fields, const string field, const string relational, const string condition){
    vectorlong used_records;
    used_records.clear();;

    //Row is a multimap based on the first field name
    int row = field_map[field];

    //Statements for the different relational strings (operators that will be passed through)
    // adding two vectors i.e. vec1 + vec2 results in all of vec2 being added to vec1
    if (relational == "="){
        if (!_indices[row].contains(condition)){
            vectorlong v;
            return v;
        }
        else {
            used_records = _indices[row][condition]; 
        }
    }
    if (relational == "<"){
        mmap_sl::Iterator it = _indices[row].begin();
        if(_indices[row].contains(condition)){
            for (; it != _indices[row].end(); it++){
                if (*it <  *(_indices[row].lower_bound(condition))){
                    used_records + (*it).value_list;
                }
            }
        }
        else{
            for (; it != _indices[row].lower_bound(condition); it++){
                used_records + (*it).value_list;
            }
        }
    }
    if (relational == ">"){
        mmap_sl::Iterator it = _indices[row].begin();
        if(_indices[row].contains(condition)){
            for (; it != _indices[row].end(); it++){
                if (*it >  *(_indices[row].lower_bound(condition))){
                    used_records + (*it).value_list;
                }
            }
        }
        else{
            it = _indices[row].lower_bound(condition);
            for (; it != _indices[row].end(); it++){
                used_records + (*it).value_list;
            }
        }
    }
    if (relational == "<="){
        mmap_sl::Iterator it = _indices[row].begin();
        mmap_sl::Iterator end_it = _indices[row].upper_bound(condition);
        end_it++;

        for (; it != end_it; it++){
            used_records + (*it).value_list;
        }
    }
    if (relational == ">="){
        mmap_sl::Iterator it = _indices[row].upper_bound(condition);
        for (; it != _indices[row].end(); it++){
            used_records + (*it).value_list;
        }
    }
    return used_records;
}


void infix_to_postfix(vector<string> &infix, queue<Token *> &postfix_queue)
{
    bool debug = false;

    // DEBUG: Print out the infix expression
    if (debug)
    {
        cout << "Here is the infix expression: ";
        for (int i = 0; i < infix.size(); i++)
        {
            cout << infix[i] << " ";
        }
        cout << endl;
    }

    stack<Token *> tokens_stack;
    string token;

    // Go through each character of the infix and create a Token based on its type
    for (int i = 0; i < infix.size(); i++)
    {
        token = infix[i];

        // Adding tokens to the stack and queue
        if (token == "<" || token == ">" || token == "<=" || token == ">=" || token == "=")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->get_presedence() <= RELATIONAL && temp->get_presedence() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty())
                    {
                        temp = tokens_stack.top();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            tokens_stack.push(new Token(token, TOKEN_OPERATOR, RELATIONAL));
        }
        else if (token == "or")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->get_presedence() <= LOGICAL_OR && temp->get_presedence() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty())
                    {
                        temp = tokens_stack.top();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            tokens_stack.push(new Token(token, TOKEN_ALPHA, LOGICAL_OR));
        }
        else if (token == "and")
        {
            if (!tokens_stack.empty())
            {
                Token *temp = tokens_stack.top();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->get_presedence() <= LOGICAL_AND && temp->get_presedence() != LPAREN)
                {
                    tokens_stack.pop();
                    postfix_queue.push(temp);

                    if (!tokens_stack.empty())
                    {
                        temp = tokens_stack.top();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            tokens_stack.push(new Token(token, TOKEN_ALPHA, LOGICAL_AND));
        }
        else if (token == "(" || token == ")")
        {
            // Token is a parenthesis
            if (token == "(") // specifically a left parenthesis
            {
                // This token can sit on any other token in the stack
                //  it can also be sat on top of by any other token
                tokens_stack.push(new Token(token, TOKEN_OPERATOR, LPAREN));
            }
            else // Token specifically a right parenthesis
            {
                // Stack is not empty and **SHOULD** contain a matching left parenthesis
                //  pop all the items until you get to the matching left paren
                Token *temp = tokens_stack.top();
                tokens_stack.pop();

                // While this token is bigger than whats on the stack, pop the smaller guy to the queue
                while (temp->get_presedence() != LPAREN && !tokens_stack.empty())
                {
                    postfix_queue.push(temp);
                    temp = tokens_stack.top();
                    tokens_stack.pop();
                }
            }
        }
        else
        { // Regular tokens go straight to the queue
            postfix_queue.push(new Token(token, TOKEN_UNKNOWN, REG_TOKEN));

            // DEBUGGING: Show me the token that is being pushed to the queue
            if (debug)
            {
                Token *temp = postfix_queue.front();
                cout << "TOKEN |" << temp->token_str() << "|" << endl;
                cout << "PREESENDENCE |" << temp->get_presedence() << "|" << endl;
            }
        }
    }
    // Infix string is done iterating, pop everything from the stack onto the queue
    while (!tokens_stack.empty())
    {
        Token *token = tokens_stack.top();
        tokens_stack.pop();
        postfix_queue.push(token);
    }
}