////////////////////////////////////////////////////////////
////
////
////
////
////
////
////
////               Basic Helper Tools 
////              (not part of Python)
////
////
////
////
////
////
////////////////////////////////////////////////////////////
        // Basic helper tools: 
            // is sub-set of a library I put together 
            // I didn't want to have 2 files so I only extracted the part 
            // of the library that the Python library needed
        // Whats in here?
            // vector overloads & helper functions
            // functions to help with strings 
            // input-output functions for all the built in types
            // functions to help with io-streams 
            // error handling (class and functions)
            // one or two (self made) debugging tools 
            // a few other things 
            



// includes
    #include <ios>                   // part of creating clear_cin
    #include <istream>               // part of creating clear_cin
    #include <iostream>              // cout , getline 
    #include <string>                // strings 
    #include <sstream>               // stringstreams 
    #include <cmath>                 // abs , pow 
    #include <stdlib.h>              // exit(0) 
    #include <stdio.h>
    #include <iomanip>               // setw , setprecision , showpoint , fixed 
    #include <limits>                // inf, max:: 
    #include <algorithm>             // min () 
    #include <vector>                // vectors 
    #include <initializer_list>      // used for overloading {}'s 

// name space 
    using namespace std;

// main
    #define main()     a; int __everything(); int main() { try { ClearScreen(); a = __everything(); return a; } catch (Errors the_error) { cerr << the_error << '\n';  } } int __everything()

// If
    #define If               if (
    #define Then                   ) { 
    #define IfNotThenIf      } else if ( 
    #define IfNot            } else { 
    #define IfNeither        } else { 
    #define IfNoneOfTheAbove } else { 
    #define EndIf            } 

// Loop 
    #define Loop_           { int Max_Value = 
    #define _Times              ; for (int LoopNumber=1; LoopNumber <= Max_Value ; LoopNumber++) {
    #define EndLoop__Times          } }
    #define Loop            while (true){
    #define EndLoop             }
    #define ExitLoop        break;
    #define GoToTopOfLoop   continue; 
    #define For_            Loop_
    #define _Vector         .size() _Times int EachItem = LoopNumber - 1;
    #define _String         .size() _Times int EachChar = LoopNumber - 1;
    #define EndFor          EndLoop__Times



// debug
    #define POKE { cout << "\nPoke\n"; string null_; getline(cin,null_); exit(0); }
     





// function declares 
    string          StreamAsString             ( istream& in );
    string          StreamStatus               ( istream& in );
    istream&        StreamFailed               ( istream& input_stream );
    bool            DidStreamFail              ( istream& input_stream );
    bool            EndOfStream                ( istream& input_stream );
    bool            StreamMatchesString        ( istream& input_stream, string  input_string );
    string          EverythingUpTo             ( char end_symbol, istream& input_stream);
    string          EverythingUpTo             ( char end_symbol);    
    string          ViewLiteral                ( string input );
    string          ViewLiteral                ( char   input );
    string          CharToString               ( char input_char );
    

    

// gobals  for indent 
    int    const INDENT_SIZE = 4;
    string const INDENT      = string(INDENT_SIZE, ' ');

////////////////////////
//
//  Functions
//
////////////////////////
    // Error handler 
        struct Errors
            {
                string info;
                Errors(string input_error) { info = input_error; }
            };
                ostream& operator<<(ostream& output_stream, const Errors& input_error)
                    {   return output_stream << input_error.info; }
                void Error( string input_error )
                    {
                        throw Errors( input_error );
                    }
                void error( string input_error )
                    {
                        throw Errors(input_error);
                    }

    // Operator Overloads
        // vector 
            // << >>
                template <class ANYTYPE> ostream& operator<<(ostream& output_stream, const vector<ANYTYPE>& input_vector)
                {
                    for ( int LoopNumber = 1 ; LoopNumber <= input_vector.size(); LoopNumber++ ) 
                        { 
                            output_stream << input_vector[LoopNumber-1] << '\n';
                        }
                    return output_stream;
                }
                template <class ANYTYPE> istream& operator>>(istream& input_stream,     vector<ANYTYPE>& input_ )
                {
                    input_ = {};
                    ANYTYPE element_holder;
                    char should_always_be_a_newline;
                    while(input_stream >> element_holder) 
                    { 
                        input_.push_back(element_holder);
                        input_stream.get(should_always_be_a_newline);
                        if (should_always_be_a_newline != '\n')
                            return StreamFailed(input_stream);
                    }
                    return input_stream;
                }
            // +    
                template <class ANYTYPE> vector<ANYTYPE> operator+(const vector<ANYTYPE>& vec1, const vector<ANYTYPE>& vec2)
                    {
                        vector<ANYTYPE> vec3 = vec1;
                        vec3.reserve(vec1.size()+vec2.size());
                        for ( ANYTYPE each : vec2 ) 
                            vec3.push_back(each);
                        return vec3;
                    }
                template <class ANYTYPE> vector<ANYTYPE>& operator+(const vector<ANYTYPE>& vec1, const ANYTYPE input_)
                    {   
                        vector<ANYTYPE> vec2 = vec1;
                        vec2.push_back(input_);
                        return vec2;
                    }
                template <class ANYTYPE> vector<ANYTYPE>& operator+( const ANYTYPE input_, const vector<ANYTYPE>& vec1)
                    {   
                        vector<ANYTYPE> vec2 = vec1;
                        vec2.insert(vec2.begin(),input_);
                        return vec2;
                    }

    // Core helper functions 
        // ClearScreen
            // code derived from 
            // http://stackoverflow.com/questions/17335816/clear-screen-using-c
            void ClearScreen()
                {
                    cout << "\033[2J\033[1;1H"; 
                } 
        // EndProgram 
            template <typename ANYTYPE> streamsize FlushStream (basic_istream   <ANYTYPE>& input_stream,   bool always_discard = false )
                {
                    //  this code is a visually modified version of the code from: 
                    //  https://www.daniweb.com/programming/software-development/threads/90228/flushing-the-input-stream
                    //  (which is a great explaination of the process ) 
                    //  it allows correct clearing of the cin buffer 
                    //  
                    //  depends on:
                    //         #include <ios>
                    //         #include <istream>
                    //         #include <limits>
                    std::streamsize num_of_chars_discarded = 0;
                    if     ( always_discard
                        || (    input_stream.rdbuf()->sungetc() != std::char_traits<ANYTYPE>::eof()
                             && input_stream.get()              != input_stream.widen ( '\n' )       ) )
                    {
                            // The stream is good, and we haven't
                            // read a full line yet, so clear it out
                            input_stream.ignore ( std::numeric_limits<std::streamsize>::max(), input_stream.widen ( '\n' ) );
                            num_of_chars_discarded = input_stream.gcount();
                    }
                    return num_of_chars_discarded;
                }
            void Pause()
                {
                    FlushStream(cin);
                    if (!cin)
                        cin.clear();
                    cin.ignore();
                } 
            void EndProgram()
                {
                    // press enter to exit
                    cout << "Press enter to exit\n";
                    Pause();
                    exit(0);
                }
        // indent           
            template <class ANYTYPE> string        Indent                     (ANYTYPE input_)
                {
                    stringstream a_stream;
                    a_stream << input_;
                    char a_char;
                    string output_string = INDENT;
                    while (a_stream.get(a_char))
                        {
                            if (a_char == '\n')
                                {
                                    output_string = output_string + "\n" + INDENT;
                                }
                            else 
                                output_string = output_string + a_char;
                        }
                    // if the last character was a \n
                    if (a_char =='\n') 
                        {
                            // remove the last indent
                            output_string = output_string.substr(0, output_string.size()-INDENT_SIZE);
                        }
                    return output_string;
                }
            string          Indent                     (istream& input_)
                {
                    string output = StreamAsString(input_);
                    return Indent(output);
                }
            string          Indent                     (stringstream& input_)
                {
                    string output = StreamAsString(input_);
                    return Indent(output);
                }
            
            // create an unindent for streams only
            // should always return a \n at the end 
            // if it returns "" it failed
            string        Input_Unindent             (istream& in)
                {

                    char char_holder;
                    string unindented_stuff ="";
                    // for each line 
                    while(true)
                        {
                            // get the first char
                            in.get(char_holder);
                            
                            // break if end of file 
                            if (in.eof()) 
                                {
                                    break;
                                }
                            // put the char back into the stream 
                            in.unget();

                            // if it doesn start with an indent, break
                            if (char_holder != ' ') 
                                {
                                    break;
                                }
                            // if for some reason the stream has a space 
                            // but is not fully indented
                            // then fail (this will mess up the stream because)
                            // the spaces cannot be (reliably) un-got
                            if (!StreamMatchesString(in, INDENT)) 
                                    {
                                        return "";
                                    }

                            // put the whole line into a string
                            string next_line = EverythingUpTo('\n',in);

                            // if there wasnt a newline at the end then fail 
                            if (next_line[next_line.size()-1] != '\n') 
                                {
                                    return "";
                                }


                            unindented_stuff += next_line;
                        }
                    return unindented_stuff;
                }
        // Stream functions 
            string          StreamAsString             ( istream& in )
                {
                    stringstream output;
                    char each_char;
                    while ( in.get(each_char) ) 
                        output << each_char;
                    string output_str = output.str();
                    return output_str;
                }
            string          StreamStatus               ( istream& in )
                {
                    string output;
                    // good
                    if (in.good())
                        output += "Good: Yes\n";
                    else 
                        output += "Good: No\n";
                    // eof
                    if (in.eof ())
                        output += "EOF : Yes\n";
                    else
                        output += "EOF : No\n";
                    // fail 
                    if (in.fail())
                        output += "Fail: Yes\n";
                    else
                        output += "Fail: No\n";
                    // bad
                    if (in.bad ())
                        output += "Bad : Yes\n";
                    else
                        output += "Bad : No\n";
                    return output;
                }
            istream&        StreamFailed               ( istream& input_stream )
                {
                    input_stream.clear();
                    input_stream.clear(ios_base::failbit);
                    return input_stream;
                }
            bool            DidStreamFail              ( istream& input_stream )
                {
                    if (input_stream.eof())
                        {
                            return false;
                        }
                    else if (input_stream.fail())
                        {
                            return true ;
                        }
                }
            bool            EndOfStream                ( istream& input_stream )
                {
                    if (input_stream.eof())
                        return true;
                    return false;
                }
            bool            StreamMatchesString        ( istream& input_stream, string  input_string )
                {
                    char char_;
                    Loop_ input_string.size() _Times
                            // if the stream runs out of characters, false 
                            if (   !(  input_stream.get(char_)  )     )
                                return false;
                            // if string doesn't match, false 
                            if ( char_ != input_string[LoopNumber-1] )
                                return false;
                    EndLoop__Times
                    return true;
                }
            string          EverythingUpTo             ( char end_symbol, istream& input_stream)
                {
                   char char_;
                   string output;
                   while (input_stream.get(char_))
                        {
                            output = output + char_ ;
                            if (char_ == end_symbol)
                                return output;
                        }
                    return output;
                }
            string          EverythingUpTo             ( char end_symbol)
                    {
                       char char_;
                       string output;
                       while (cin.get(char_) )
                            {
                                output = output + char_ ;
                                if (char_ == end_symbol)
                                    return output;
                            }
                        return output;
                    }
        // String helpers   
            string       Literal    ( string input )
                {
                    char each_char;
                    stringstream char_stream;
                    char_stream << input;
                    string output;
                    while (char_stream.get(each_char))
                        {
                            if (each_char == '\n')
                                output += "\\n"; 
                            else if (each_char == '\\')
                                output += "\\\\";
                            else if (each_char == '\t')
                                output += "\\t";
                            else if (each_char == '\0')
                                output += "\\0";
                            else if (each_char == '\r')
                                output += "\\r";
                            else if (each_char == '\v')
                                output += "\\v";
                            else if (each_char == '\b')
                                output += "\\b";
                            else 
                                output += each_char;
                        }
                    return output;
                }
            string       Literal    ( char   input )
                {
                    stringstream output;
                    output << input;
                    string output_str = output.str();
                    return Literal(output_str);
                }
            string   ViewLiteral    ( string input )
                {
                    char each_char;
                    stringstream char_stream;
                    char_stream << input;
                    string output;
                    int space_counter = 0;
                    int newline_counter = 0;
                    while (char_stream.get(each_char))
                        {
                            // spaces 
                            if (each_char == ' ')
                                { 
                                    space_counter++;
                                    if (char_stream.peek() != ' ')
                                        {
                                            output += "\\"+to_string(space_counter)+"SPACES";
                                            space_counter = 0;
                                        }
                                }
                            // newlines
                            else if (each_char == '\n')
                                {
                                    newline_counter++;
                                    if (char_stream.peek() != '\n')
                                        {
                                            if (newline_counter > 1)
                                                output += "*"+to_string(newline_counter)+"_NEWLINES*\n";     
                                            else
                                                output += "\\n\n"; 
                                            newline_counter = 0;
                                        }
                                }
                            // everything else 
                            else if (each_char == '\\')
                                output += "\\\\";
                            else if (each_char == '\t')
                                output += "\\t";
                            else if (each_char == '\0')
                                output += "\\0";
                            else if (each_char == '\r')
                                output += "\\r";
                            else if (each_char == '\v')
                                output += "\\v";
                            else if (each_char == '\b')
                                output += "\\b";
                            else 
                                output += each_char;
                        }
                    return output;
                }
            string   ViewLiteral    ( char   input )
                {
                    stringstream output;
                    output << input;
                    string output_str = output.str();
                    return ViewLiteral(output_str);
                }
            string   CharToString   ( char input_char )
                { 
                    stringstream stream_for_the_char;
                    stream_for_the_char << input_char;
                    return stream_for_the_char.str();
                }
        // IO basic types 
            // CopyPaste Class IO
                // this isn't useful by itself 
                // but if you replace classname 
                // with the name of a class you create
                // and change data_members to corrispond 
                // to the data members of the class 
                // then it is really useful 

                //ostream& output (ostream& out, const classname& input_)
                //    {
                //        // name of class 
                //        out << "classname" << '\n';
                //        
                //                stringstream data_members;
                //                
                //        // for each datamember
                //        output( data_members , input_.datamember()); data_members << '\n';
                //        
                //                // format and return stream
                //                out << Indent( data_members.str() ) << '\n';
                //                return out;
                //    }
                //istream& input  (istream& in,        classname& input_)
                //    {
                //    
                //        // check name of type 
                //        if ( EverythingUpTo('\n',in) != "classname\n")
                //            return StreamFailed(in);
                //        
                //  
                //                // unindent members
                //                string content = Input_Unindent(in);
                //                // check fail
                //                if (content == "")
                //                    return StreamFailed(in);
                //                // check end 
                //                if (content[content.size()-1] != '\n')
                //                    return StreamFailed(in);
                //                // setup vars
                //                char should_always_be_a_newline;
                //                stringstream transfer_method;
                //                // get rid of the class's newline
                //                transfer_method << content.substr(0,content.size()-1);
                //            
                //    
                //        // create empty versions of all data members 
                //        datatype datamember;
                //        input( transfer_method, datamember ); transfer_method.get(should_always_be_a_newline);
                //            
                //  
                //                // if stream fails, then return fail 
                //                if (DidStreamFail(transfer_method))
                //                    return StreamFailed(in);
                //            
                // 
                // 
                //        // check the data, make sure its valid 
                //        if ( !IsDataValid(datamember)) 
                //            return StreamFailed(in);
                //
                //        // add the data to the input 
                //        input_.data = datamember
                //    
                //                // if everything worked return the stream! 
                //                return in;
                //    }  
            // bool IO        
                ostream& output(ostream& out, const bool& input)
                    {
                        // name of class 
                        out << "bool" << '\n';
                        // each data member in class
                        if (input)
                            out << Indent("true") << '\n';
                        else 
                            out << Indent("false") << '\n';
                        return out;
                    }
                istream& input (istream& in,        bool& input)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "bool\n")
                                return StreamFailed(in);
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // try to input data
                            if      (content == "true\n")
                                input = true;
                            else if (content == "false\n")
                                input = false;
                            // if not yes or no, then fail
                            else 
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }  
            // char IO        
                ostream& output(ostream& out, const char& input_char)
                    {
                        // name of class 
                        out << "char" << '\n';
                        // each data member in class
                        out << Indent(input_char) << '\n';
                        return out;
                    }
                istream& input (istream& in,        char& input_char)

                        {
                            // reset the char
                            input_char = '\0';

                            // check name of class 
                            if ( EverythingUpTo('\n',in) != "char\n")
                                return StreamFailed(in);
                            // unindent everything
                            string char_content = Input_Unindent(in);
                            // check fail
                            if (char_content == "")
                                return StreamFailed(in);
                            // try to input data
                            if (char_content.size() == 2)
                                {
                                    if (char_content[1] == '\n')
                                        input_char = char_content[0];
                                    else 
                                        return StreamFailed(in);
                                }
                            else 
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }
            // int IO         
                ostream& output(ostream& out, const int& input)
                    {
                        // name of class 
                        out << "integer" << '\n';
                        // each data member in class
                        out << Indent(input) << '\n';
                        return out;
                    }
                istream& input (istream& in,        int& input)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "integer\n")
                                return StreamFailed(in);
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // try to input data
                            stringstream content_as_stream;
                            content_as_stream << content;
                            char should_always_be_a_newline;
                            content_as_stream >> input; 
                            content_as_stream.get(should_always_be_a_newline);
                            // if failed somewhere, then return fail
                            if (DidStreamFail(content_as_stream)) 
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }            
            // double IO      
                ostream& output(ostream& out, const double& input)
                    {
                        // name of class 
                        out << "double" << '\n';
                        // make sure and get all the decimals 
                        stringstream output;
                        output << setprecision(20) << input;
                        // each data member in class
                        out << Indent(output.str()) << '\n';
                        return out;
                    }
                istream& input (istream& in,        double& input)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "double\n")
                                return StreamFailed(in);
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // try to input data
                            stringstream content_as_stream;
                            content_as_stream << content;
                            char should_always_be_a_newline;
                            content_as_stream >> input; 
                            content_as_stream.get(should_always_be_a_newline);
                            // if failed somewhere, then return fail
                            if (DidStreamFail(content_as_stream)) 
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }            
            // long double IO 
                ostream& output(ostream& out, const long double& input)
                    {
                        // name of class 
                        out << "double" << '\n';
                        // make sure and get all the decimals 
                        stringstream output;
                        output << setprecision(20) << input;
                        // each data member in class
                        out << Indent(output.str()) << '\n';
                        return out;
                    }
                istream& input (istream& in,        long double& input)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "double\n")
                                return StreamFailed(in);
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // try to input data
                            stringstream content_as_stream;
                            content_as_stream << content;
                            char should_always_be_a_newline;
                            content_as_stream >> input; 
                            content_as_stream.get(should_always_be_a_newline);
                            // if failed somewhere, then return fail
                            if (DidStreamFail(content_as_stream)) 
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }            
            // string IO      
                ostream& output(ostream& out, const string& input)
                    {
                        // name of class 
                        out << "string" << '\n';
                        // each data member in class
                        out << Indent(input) << '\n';
                        return out;
                    }
                istream& input (istream& in,        string& input)
                        {
                            // check name of class 
                            if ( EverythingUpTo('\n',in) != "string\n")
                                return StreamFailed(in);
                            
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // if it doesn't end in newline, then fail
                            if (content[content.size()-1] != '\n')
                                return StreamFailed(in);
                            // try to input data
                            input = content.substr(0,content.size()-1);
                            // if everything worked return the stream! 
                            return in;
                        }                            
                // c-string IO 
                    ostream& output(ostream& out, const char input[])
                        {
                            // name of class 
                            out << "string" << '\n';
                            // each data member in class
                            out << Indent(input) << '\n';
                            return out;
                        } 
            // general IO     
                template <class ANYTYPE> ostream& output(ostream& out, const ANYTYPE& input)
                    {
                         // name of class 
                        out << "default IO" << '\n';
                        // each data member in class
                        out << Indent(input) << '\n';
                        return out;
                    }
                template <class ANYTYPE> istream& input (istream& in,        ANYTYPE& input)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "default IO\n")
                                return StreamFailed(in);
                            // unindent everything
                            string content = Input_Unindent(in);
                            // check fail
                            if (content == "")
                                return StreamFailed(in);
                            // check end 
                            if (content[content.size()-1] != '\n')
                                return StreamFailed(in);
                            // store data
                            stringstream transfer_method;
                            transfer_method << content.substr(0,content.size()-1);
                            transfer_method >> input;
                            // if stream fails, then return fail 
                            if (DidStreamFail(transfer_method))
                                return StreamFailed(in);
                            // if everything worked return the stream! 
                            return in;
                        }              
            // Conversion     
                template <class ANYTYPE> string AsString(const ANYTYPE& input)
                    {
                        stringstream data_stream;
                        output(data_stream, input);
                        // get rid of the name 
                        EverythingUpTo('\n', data_stream);
                        // get the content
                        string content = Input_Unindent(data_stream);
                        // remove the closing newline
                        content = content.substr(0,content.size()-1);
                        return content;
                    }
                template <class ANYTYPE> string TypeAsString(const ANYTYPE& input)
                    {
                        stringstream data_stream;
                        output(data_stream, input);
                        // get the name of the datatype
                        string name = EverythingUpTo('\n', data_stream);
                        // remove the closing newline
                        name = name.substr(0,name.size()-1);
                        return name;
                    }
        // VisualFormat basic types
            // general VisualFormat
                // for most things just output via their << operator
                template<class ANYTYPE> 
                string VisualFormat (ANYTYPE input) 
                    {
                        stringstream out;
                        out << input;
                        return out.str();
                    }
            // bool   
                string VisualFormat (bool   input)
                    {
                        string output;
                        if (input == true)
                            output = "true";
                        else
                            output = "false";
                        return output;
                    }
            // char   
                string VisualFormat (char   input)
                    {
                        return CharToString(input);
                    }
            // int    
                string VisualFormat (int    input)
                    {
                        return to_string(input);
                    }
            // double 
                string VisualFormat (double input)
                    {
                        return to_string(input);
                    }
            // string 
                string VisualFormat (string input)
                    {
                        return input;
                    }
        // Show     
            template<class ANYTYPE> string Show(ANYTYPE input)
                {
                    cout << VisualFormat(input);
                    return VisualFormat(input);
                }
            template<class ANYTYPE> string ShowLine(ANYTYPE input)
                        {
                            cout << VisualFormat(input) << '\n';
                            return VisualFormat(input) + '\n';
                        }


//////////////////////
//
// Secondary helper functions
//
//////////////////////
    // Vector functions 
        // IO           
            template <class    ANYTYPE> ostream&   output   (ostream& out, const vector<ANYTYPE>& input_vector)
                {
                    // name of class 
                    out << "vector" << '\n';
                    // each data member in class
                    stringstream all_members;
                    for ( ANYTYPE each : input_vector ) 
                        { 
                            output(all_members, each);
                            all_members << '\n';
                        }
                    // if no data members
                    if (input_vector.size() == 0)
                        all_members << "\n";
                    
                    // Indent the whole thing
                    string all_members_str = all_members.str();
                    out << Indent(all_members_str) << '\n';
                    return out;
                }
            template <class    ANYTYPE> istream&   input    (istream& in, vector<ANYTYPE>& input_vector)
                {

                    // reset the input vector
                    input_vector = {};
                    
                    // check name of class 
                    if ( EverythingUpTo('\n',in) != "vector\n")
                        return StreamFailed(in);
                    // unindent everything
                    string vector_content = Input_Unindent(in);
                    // check fail
                    if (vector_content == "")
                        return StreamFailed(in);
                    // check no members
                    if ( vector_content == "\n" )
                        return in;
                    // try to input data
                    ANYTYPE data_member;
                    stringstream vector_content_as_stream;
                    vector_content_as_stream << vector_content;
                    char should_always_be_a_newline;
                    // try inputting data members till EOF 
                    do 
                        {
                            input(vector_content_as_stream, data_member);
                            vector_content_as_stream.get(should_always_be_a_newline);
                            if ( DidStreamFail(vector_content_as_stream) )
                                {
                                    return StreamFailed(in);
                                }
                            input_vector.push_back(data_member);
                        } while(not EndOfStream(vector_content_as_stream) );
                    // if no data members were added, report an error
                    if (input_vector.size() == 0)
                        return StreamFailed(in);
                    // if everything worked return the stream! 
                    return in;
                }
        // VisualFormat 
            template <class    ANYTYPE> string          VisualFormat                  (vector<ANYTYPE> input)
                {
                    stringstream output;
                    for ( ANYTYPE each : input ) 
                        {
                            output << VisualFormat(each) << '\n';
                        }
                    return Indent(output.str());
                }
        // helpers      
            template <class ANYTYPE> void             Remove__ThElementFrom__Vector (int element_number  , vector<ANYTYPE>& input_vector ) 
                { 
                    input_vector.erase(input_vector.begin() + (int)element_number);
                } 
            template <class ANYTYPE> bool             Is__In__Vector                (ANYTYPE test_val    , vector<ANYTYPE>& input_vector ) 
                { 
                    For_ input_vector _Vector 
                        If input_vector.at( EachItem ) == test_val Then 
                            return true ;
                        EndIf
                    EndFor 
                    return false ; 
                } 
            template <class ANYTYPE> bool             Is__In__                      (ANYTYPE test_val    , vector<ANYTYPE>& input_vector ) 
                { 
                    For_ input_vector _Vector 
                        If input_vector.at( EachItem ) == test_val Then 
                            return true ;
                        EndIf
                    EndFor 
                    return false ; 
                } 
            template <class ANYTYPE> int              IndexOf__In__Vector           (ANYTYPE test_val    , vector<ANYTYPE>& input_vector ) 
                { 
                    For_ input_vector _Vector 
                        If (input_vector.at( EachItem ) == test_val) Then 
                            return EachItem;
                        EndIf
                    EndFor 
                    return -1; 
                } 
            template <class ANYTYPE> void             RemoveFirst__From__Vector     (ANYTYPE element_    , vector<ANYTYPE>& input_vector ) 
                { 
                    int element_number = IndexOf__In__Vector(element_ , input_vector);
                    input_vector.erase(input_vector.begin() + element_number);
                } 
            template <class ANYTYPE> int              NumberOf__In__Vector          (ANYTYPE test_val    , vector<ANYTYPE>& input_vector ) 
                { 
                    int number_of_matches;
                    For_ input_vector _Vector 
                        If input_vector.at( EachItem ) == test_val Then 
                            number_of_matches++;
                        EndIf
                    EndFor 
                    return number_of_matches; 
                } 
            template <class ANYTYPE> vector<ANYTYPE>& RemoveDuplicates              (                      vector<ANYTYPE>& input_vector ) 
                { 
                    vector<ANYTYPE> search_vector; 
                    For_ input_vector _Vector 
                            If Is__In__Vector( input_vector.at(EachItem) , search_vector ) Then
                                Remove__ThElementFrom__Vector(EachItem, input_vector);
                            IfNot
                                search_vector.push_back( input_vector.at(EachItem) );
                            EndIf 
                    EndFor 
                }
            template <class ANYTYPE> vector<ANYTYPE>& sort( vector<ANYTYPE>& input_ )
                {
                    sort(input_.begin(),input_.end());
                    return input_;
                }
            template <class ANYTYPE> vector<ANYTYPE>& Sort( vector<ANYTYPE>& input_ )
                {
                    sort(input_.begin(),input_.end());
                    return input_;
                }
            template <class ANYTYPE> vector<ANYTYPE>& Reverse( vector<ANYTYPE>& input_ )
                {
                    reverse(input_.begin(),input_.end());
                    return input_;
                }
            template <class ANYTYPE> vector<ANYTYPE>& remove( ANYTYPE value_, vector<ANYTYPE>& input_ )
                {
                    remove(input_.begin(),input_.end(), value_);
                    return input_;
                }






////////////////////////////////////////////////////////////
////
////
////
////
////
////
////
////       End Basic Helper Tools 
////
////
////
////
////
////
////
////////////////////////////////////////////////////////////




















////////////////////////////////////////////////////////////
////
////
////
////
////
////
////
////       Python! 
////
////
////
////
////
////
////
////////////////////////////////////////////////////////////
            // whats in here?
                // print keyword 
                // For()in() keyphrase 
                // List class
                // Item class
                // generic helper functions



            // Future things to add:
                // List
                    // make a SaveIn() and LoadFrom() method
                    // add a .join() function 
                    // add a split() function 
                    // add some regex functions
                    // add mathmatical functions for List 
                        // Average()
                        // Sum()
                // Item 
                    // completely re-work the code using decltype(), pointers and dynamic memory
                    // minimize float precision errors with Item
                    // allow for high accuracy numbers (BigNum)
                    // make an infinite-type inside of the number type 
                // make a Grid class (will be like a matrix) 

                

 



//////////////////////////////
//
// print 
//
//////////////////////////////

        // I made this code based on code from the site below
        // I like to think I made it more useful, but all originalality 
        // goes to the original author
        // http://wiki.c2.com/?OverloadingCommaOperator
        
        // to print
        #define print __PrintOutputFixerStream.reset();__PrintOutputFixerStream,
        class __PrintOutputFixerStreamClass : public stringstream
            {
                protected:
                    // data 
                        int length_of_last_line = 0;
                        bool dont_go_up_a_line = true;
                        string what_was_just_output = "";
                        string content = "";
                    // class helper functions
                        void go_up_a_line()
                            {
                                cout << "\033[1A";
                            }
                        void go_to_the_right(int number_of_spaces)
                            {
                                cout << "\033["+to_string(number_of_spaces)+"C";
                            }
                        void save_cursor_position()
                            {
                                cout << "\033[s";
                            }
                        void restore_cursor_position()
                            {
                                cout << "\033[u";
                            }
                        int length_of_previous_line()
                            {
                                int charaters_till_newline = 0;
                                Loop_ what_was_just_output.size() _Times
                                    if (what_was_just_output[what_was_just_output.size() - LoopNumber] != '\n')
                                        {
                                            charaters_till_newline++;
                                        }
                                    else 
                                        {
                                            break;
                                        }
                                EndLoop__Times
                                return charaters_till_newline;
                            }
                public:
                    void reset()
                        {
                            length_of_last_line = 0;
                            dont_go_up_a_line = true;
                            what_was_just_output = "";
                        }
                    void output_line()
                        {
                            if (dont_go_up_a_line)
                                {
                                    save_cursor_position();
                                    cout << content << "\n";
                                    dont_go_up_a_line = false;
                                }
                            else
                                {
                                    go_up_a_line();
                                    length_of_last_line += length_of_previous_line();
                                    go_to_the_right(length_of_last_line);
                                    cout << content; // dont output a newline
                                    // FIXME, if the terminal width is really small 
                                    // and the string is really long, this might mess things up 
                                }
                            restore_cursor_position();
                        }
                    void add_content(string input_)
                        {
                            what_was_just_output = content;
                            content = input_;
                        }
            };
        __PrintOutputFixerStream __PrintOutputFixerStream;
        template<class ANYTYPE>
        // for normal input (classes)
        inline ostream& operator,(ostream& o, const ANYTYPE& value) 
            {
                // if its from print, then use the __PrintOutputFixerStream to add a newline
                if (&o == &__PrintOutputFixerStream)
                    {
                        // erase the string-stream part of __PrintOutputFixerStream
                        __PrintOutputFixerStream.str("");
                        // add the line to __PrintOutputFixerStream
                        __PrintOutputFixerStream.add_content(VisualFormat(value));
                        // output the line 
                        __PrintOutputFixerStream.output_line();
                    }
                // if it's some other stream
                else
                    {
                        // then just keep going like normal
                        o << VisualFormat(value);
                    }
                return o;
            }
        // for stream operators (fixed, setprecision(), etc)
        inline ostream& operator,(ostream& o, ostream& (*manip_fun)(ostream&)) 
            {
                // if its from print, then use then send it to cout directly 
                if (&o == &__PrintOutputFixerStream)
                    {
                        cout << manip_fun;
                    }
                // if it's some other stream
                else
                    {
                        // then just keep going like normal
                        o << manip_fun;
                    }
                return o;
            }









//////////////////////////////
//
// For()in() 
//
//////////////////////////////
        #define For(ARG) for (auto ARG
        #define in(ARG)   : ARG.Vector() )
        // pretty simple huh?







//////////////////////////////
//
// List
//
//////////////////////////////
        template <typename ITEM> 
        class List
            {
                // data
                    vector<ITEM>    vector_ ;
                    vector<ITEM>    values  ;
                    vector<string>  names   ; 


            public:
                // member functions 
                    // NumberOfItems    
                        int NumberOfItems() const 
                            { 
                                return vector_.size()+names.size(); 
                            }
                    // Sample           
                        string  Sample() const
                            { 
                                // if theres nothing in the list, return nothing 
                                if (vector_.size() == 0)
                                    return "None";
                                short int first_few = (vector_.size() > 3) ? 3 : vector_.size();
                                string output_string = "";
                                Loop_ first_few _Times
                                    output_string = output_string + vector_.at(LoopNumber-1).Data() + '\n' ;
                                EndLoop__Times
                                return output_string;
                                
                            }
                    // at               
                        ITEM& at (int position )
                            {
                                bool is_in_bounds = abs(position) <= vector_.size() && position != 0;
                                bool is_negative  = position      < 0;
                                If is_in_bounds Then 
                                    If is_negative Then 
                                        // if -1 then return the last element 
                                        // if -2 then return 2nd-to-last element 
                                        // etc 
                                        return vector_.at(vector_.size() + position);
                                    IfNot 
                                        return vector_.at(position-1);
                                    EndIf 
                                
                                IfNot 
                                    If is_negative Then 
                                        Error(  "Somewhere the code is asking for item "+to_string(position)+" in a list\b"
                                               +"But that item doesn't exist :/\n" 
                                               +"Here is a sample of the list:\n"
                                               +this->Sample() );
                                    // if the position doesnt exist, then create it (and all of the inbetween values)
                                    IfNot
                                        int missing_items = position - vector_.size();
                                        Loop_ missing_items _Times 
                                            vector_.push_back(ITEM());
                                        EndLoop__Times
                                        return vector_.at(position-1);
                                    EndIf
                                EndIf 
                                
                            }
                    
                        ITEM& at (string name )
                            {
                                return (*this)[name];
                            }
                    // size             
                        int size() const 
                            {
                                return this->NumberOfItems();
                            }
                    // length           
                        int length() const 
                            {
                                return vector_.size();
                            }
                    // add              
                        template <class ANYTYPE> void add(const ANYTYPE& input )
                            {
                                vector_ .push_back( ITEM(input) );
                            }
                    // append           
                        template <class ANYTYPE> void append(const ANYTYPE& input )
                            {
                                vector_.push_back( ITEM(input) );
                            }
                    // sort             
                            void sort()
                                {
                                    Sort(vector_);
                                }
                    // reverse          
                            void reverse ()
                            {
                                Reverse(vector_);
                            }
                    // insert           
                        template <class ANYTYPE>
                        void insert ( const ANYTYPE& input_ , const long int& index_)
                            {  
                                // 0-indexed
                                vector_.insert( vector_.begin() + index_ , ITEM(input_));
                            }
                    // insert__at__     
                        template <class ANYTYPE>
                        void insert__at__ ( const ANYTYPE& input_ , const long int& index_)
                            {  
                                // 1-indexed 
                                vector_.insert( vector_.begin() + index_ - 1, ITEM(input_));
                            }
                    // RemoveItemAt     
                        void RemoveItemAt  (const long int& item_position  )
                            {
                                If item_position > vector_.size() or item_position < 1 Then 
                                    Error( "Somewhere there's a command trying to use RemoveItemAt() with an out-of-bounds value\n" );
                                EndIf 
                                Remove__ThElementFrom__Vector((item_position -1), vector_);
                            } 
                    // RemoveItemNamed  
                        void RemoveItemNamed  (const string& name  )
                            {
                                For_ names _Vector
                                    if ( name == names.at(EachItem) )
                                        {
                                            Remove__ThElementFrom__Vector(EachItem, names );
                                            Remove__ThElementFrom__Vector(EachItem, values);
                                            // should be able to end because there should be no duplicate name entrys 
                                            return;
                                        }
                                EndFor

                            }
                    // RemoveAll        
                        template <class ANYTYPE> void RemoveAll  (const ANYTYPE& input_  )
                            {
                                
                                ITEM test_val = ITEM(input_);
                                // remove values from vector_ 
                                remove(ITEM(input_), vector_);
                                
                                // remove values from named values  
                                Loop_  values.size()  _Times
                                    // needs to be done backwards or there will be indexing errors 
                                    int EachItemInReverse  = values.size() - LoopNumber;
                                    if ( test_val == values.at(EachItemInReverse) )
                                        {
                                            Remove__ThElementFrom__Vector(EachItemInReverse, names );
                                            Remove__ThElementFrom__Vector(EachItemInReverse, values);
                                        }
                                EndLoop__Times

                            } 
                    // RemoveDuplicates 
                            void RemoveDuplicates()
                                {
                                    RemoveDuplicates(vector_);
                                }
                    // From__To__       
                        List<ITEM>     From__To__      (int starting_spot, int ending_spot ) 
                            {
                                // if the inputs are negative, flip them
                                if ( starting_spot < 0 )
                                    starting_spot = vector_.size() + starting_spot+1; 
                                if ( ending_spot < 0 )
                                    ending_spot = vector_.size() + ending_spot+1; 

                                // out-of-bounds errors
                                if ( ending_spot > vector_.size() )
                                    error("Sorry but I think somewhere theres a From__To__, \nbut the 'To__' input is larger than the List");
                                if ( starting_spot == 0)
                                    error("Sorry but I think somewhere theres a From__To__, \nbut the 'From__' input is 0 \n(and the From__To__ function is 1-indexed)");
                                
                                List<ITEM> output_list; 
                                Loop_  (ending_spot - starting_spot ) + 1 _Times 
                                        output_list.add(vector_.at(LoopNumber-1+starting_spot-1));
                                EndLoop__Times
                                return output_list; 
                            }
                    // Data             
                        vector<ITEM> Data () const 
                            { 
                                return vector_ + values;   
                            }
                    // Vector           
                        // you may ask why are there two functions here 
                        // well the default one allows for changes to the internal vector 
                        // (Which is fine) 
                        // the non-default one is for the << operator 
                        // because it requires a const function 
                        vector<ITEM>& Vector ()  
                            { 
                                return vector_;   
                            }
                        vector<ITEM> Vector () const
                            { 
                                return vector_;   
                            }
                    // Names            
                        vector<string> Names() const
                            {
                                    return names;
                            }
                    // NamedValues      
                        vector<ITEM> NamedValues () const 
                            { 
                                return values;   
                            }
                    // Is__AName        
                            template<class ANYTYPE> bool Is__AName(ANYTYPE input_)
                                {
                                    // if its not a string then it can't be a name 
                                    ITEM input_as_item = ITEM(input_);
                                    If input_as_item.Type() != "string" Then 
                                        return false;
                                    EndIf 
                                    return Is__In__Vector(input_as_item.Data(), names);
                                }
                    // Is__ANamedValue  
                                template <class ANYTYPE> bool Is__ANamedValue(ANYTYPE input)
                                    {
                                        return Is__In__Vector(ITEM(input),values);
                                    }
                    // Is__AVectorValue 
                                template <class ANYTYPE> bool Is__AVectorValue(ANYTYPE input)
                                    {
                                        return Is__In__Vector(ITEM(input),vector_);
                                    }
                    // Is__AValue       
                                template <class ANYTYPE> bool Is__AValue(ANYTYPE input)
                                    {
                                        if ( Is__In__Vector(ITEM(input),values) )
                                            return true;
                                        if ( Is__In__Vector(ITEM(input),vector_) )
                                            return true;
                                        return false;
                                    }
                    // IndexOf          
                            template <class ANYTYPE> 
                            ITEM IndexOf(ANYTYPE input_)
                                {
                                    ITEM input_as_item(input_);
                                    int normal_vector_index = IndexOf__In__Vector( input_as_item , vector_ );
                                    // if valid, return it 
                                    if (normal_vector_index >= 0)
                                        // the +1 is for 1-indexing 
                                        return ITEM(normal_vector_index+1);
                                    else
                                        {
                                            int named_index = IndexOf__In__Vector( input_as_item , values );
                                            // if valid, find the corrisponding name
                                            if ( named_index >= 0 )
                                                {
                                                    return names.at(named_index);
                                                }
                                            else
                                                // if not found, return the nothing item
                                                return ITEM(); 
                                        }
                                } 
                            template <class ANYTYPE> 
                            ITEM index(ANYTYPE input_)
                                {
                                    ITEM input_as_item(input_);
                                    int normal_vector_index = IndexOf__In__Vector( input_as_item , vector_ );
                                    // if valid, return it 
                                    if (normal_vector_index >= 0)
                                        return ITEM(normal_vector_index);
                                    else
                                        {
                                            int named_index = IndexOf__In__Vector( input_as_item , values );
                                            // if valid, find the corrisponding name
                                            if ( named_index >= 0 )
                                                {
                                                    return names.at(named_index);
                                                }
                                            else
                                                // if not found, return the nothing item
                                                return ITEM(); 
                                        }
                                } 
                    // IndexesOf        
                            template <class ANYTYPE> 
                            List<ITEM> IndexesOf(ANYTYPE input_)
                                {
                                    ITEM input_as_item(input_);
                                    List indexes;
                                    // if valid, return it 
                                    For_ vector_ _Vector 
                                        if (vector_.at(EachItem) == input_as_item)
                                            // the +1 is for 1-indexing
                                            indexes.add(EachItem+1);
                                    EndFor 
                                    For_ values _Vector
                                        if (values.at(EachItem) == input_as_item)
                                            indexes.add(names.at(EachItem));
                                    EndFor 
                                    return indexes; 
                                } 
                            template <class ANYTYPE> 
                            List<ITEM> indexes(ANYTYPE input_)
                                {
                                    ITEM input_as_item(input_);
                                    List indexes;
                                    // if valid, return it 
                                    For_ vector_ _Vector 
                                        if (vector_.at(EachItem) == input_as_item)
                                            indexes.add(EachItem);
                                    EndFor 
                                    For_ values _Vector
                                        if (values.at(EachItem) == input_as_item)
                                            indexes.add(names.at(EachItem));
                                    EndFor 
                                    return indexes; 
                                } 
                    // Has              
                            template <class ANYTYPE> bool Has(ANYTYPE input)
                                    {
                                        return Is__AName(ITEM(input)) || Is__ANamedValue(ITEM(input)) || Is__AValue(ITEM(input));
                                    }
                    // SizeOfLargestName
                        int SizeOfLargestName() const
                            {
                                int largest_size =0;
                                for( auto const& each : names )
                                    {
                                        if (each.size() > largest_size)
                                            {
                                                largest_size = each.size();
                                            }
                                    }  
                                return largest_size;
                            }
                    // Input            
                        istream& Input  (istream& in)
                            {
                            
                                // check name of type 
                                if ( EverythingUpTo('\n',in) != "List\n")
                                    return StreamFailed(in);
                                
                          
                                        // unindent members
                                        string content = Input_Unindent(in);
                                        // check fail
                                        if (content == "")
                                            return StreamFailed(in);
                                        // check end 
                                        if (content[content.size()-1] != '\n')
                                            return StreamFailed(in);
                                        // setup vars
                                        char should_always_be_a_newline;
                                        stringstream transfer_method;
                                        // get rid of the class's newline
                                        transfer_method << content.substr(0,content.size()-1);
                                    
                                
                                // create empty versions of all data members 
                                input( transfer_method , vector_ ); transfer_method.get(should_always_be_a_newline);
                                input( transfer_method , values  ); transfer_method.get(should_always_be_a_newline);
                                input( transfer_method , names   ); transfer_method.get(should_always_be_a_newline);
                          
                                        // if stream fails, then return fail 
                                        if (DidStreamFail(transfer_method))
                                            return StreamFailed(in);
                        
                            
                            
                                        // if everything worked return the stream! 
                                        return in;
                            }  
                
                // Constructors 
                    List<ITEM> (  ) { }
                    List<ITEM> ( ITEM input_ ) 
                        { 
                            if (input_.Type() == "List")
                                {    
                                    stringstream transfer_method;
                                    transfer_method << input_.Data();   
                                    char should_always_be_a_newline;
                                    // input the data
                                    input( transfer_method , vector_ ); transfer_method.get(should_always_be_a_newline);
                                    input( transfer_method , values  ); transfer_method.get(should_always_be_a_newline);
                                    input( transfer_method , names   ); transfer_method.get(should_always_be_a_newline);
                                }
                            else 
                                error("I think you're trying to assign a non-list var to a list");
                        }
                    template <class ANYTYPE>
                    List<ITEM> (  vector<ANYTYPE> vec ) 
                        { 
                            vector_ = vec;
                        }    
                    template <class ANYTYPE> List<ITEM> (  initializer_list<ANYTYPE> list ) 
                        { 
                            for( auto each : list  )
                                {
                                    vector_.push_back(ITEM(each));   
                                }
                        }    
                    List<ITEM> ( string input_ ) 
                        { 
                            For_ input_ _String
                                this->add(ITEM(input_[EachChar]));
                            EndFor
                        }
                    List<ITEM> ( const char input_[] ) 
                        { 
                            string input_as_string = input_;
                            For_ input_as_string _String
                                this->add(ITEM(input_as_string[EachChar]));
                            EndFor
                        }
                // internal overloads
                    ITEM& operator[] (long int position) 
                        {
                            // if positive, 0-index it
                            if (position >= 0)
                                return this->at(position+1);
                            else 
                                return this->at(position);
                        }
                    ITEM& operator[] (string   name) 
                        {
                            // if name is in names, return the value
                            long int index_ = IndexOf__In__Vector(name, names);
                            if ( index_ != -1 )
                                {
                                    return values.at(index_);
                                }
                            else 
                                {
                                    // add the name 
                                    names .push_back(  name  );
                                    values.push_back( ITEM() );
                                    // return the Item 
                                    return values.at(values.size()-1);
                                }
                        }
                    template <class ANYTYPE> 
                    List<ITEM>& operator=  (vector<ANYTYPE> const &assignment_data) 
                                { 
                                    vector_ = {};
                                    For_ assignment_data _Vector 
                                        vector_.push_back(     Item(  assignment_data.at(EachItem)  )     );
                                    EndFor 
                                    return *this;
                                }
                    List<ITEM>& operator=  (const ITEM&         assignment_data) 
                                { 
                                    if (assignment_data.Type() == "List")
                                        {    
                                            stringstream in;
                                            in << assignment_data.Data();
                                            // import list 
                                            this->Input(in);
                                            return *this;
                                        }
                                    else 
                                        error("I think you're trying to assign a non-list var to a list");
                                    return *this;
                                }
                    List<ITEM>& operator=  (const vector<ITEM>& assignment_data) 
                                { 
                                    vector_ = assignment_data; 
                                    return *this;
                                }
                    template <class ANYTYPE> 
                    List<ITEM>& operator=  ( const initializer_list<ANYTYPE> list ) 
                        { 
                            for( auto each : list  )
                                {
                                    vector_.push_back(ITEM(each));   
                                }
                        }    
                    List<ITEM>& operator=  ( const string input_ ) 
                        { 
                            For_ input_ _String
                                this->add(ITEM(input_[EachChar]));
                            EndFor
                        }
                    List<ITEM>& operator=  ( const char input_[] ) 
                        { 
                            string input_as_string = input_;
                            For_ input_as_string _String
                                this->add(ITEM(input_as_string[EachChar]));
                            EndFor
                        }
                    // implicit conversion 
                        class Item;
                        operator vector<Item>() const 
                            {
                                    return vector_;
                            }
            };
                // external overloads 
                    // IO          
                        template <class ITEM> ostream& output (ostream& out, const List<ITEM>& input_)
                            {
                                // name of class 
                                out << "List" << '\n';
                                
                                        stringstream data_members;
                                        
                                // for each datamember
                                output( data_members , input_.Vector     ()); data_members << '\n';
                                output( data_members , input_.Names      ()); data_members << '\n';
                                output( data_members , input_.NamedValues()); data_members << '\n';
                                
                                        // format and return stream
                                        out << Indent( data_members.str() ) << '\n';
                                        return out;
                            }
                        template <class ITEM> istream& input  (istream& in,        List<ITEM>& input_)
                            {
                                return input_.Input(in);
                            }  
                    // VisualFormat
                            template <typename ITEM> string VisualFormat( const List<ITEM>& input_)
                                {

                                    // output both
                                    if ( input_.NamedValues().size() >= 1 and input_.Vector().size() >= 1 ) 
                                        
                                        {
                                            stringstream named_values_stream;
                                            For_ input_.Names() _Vector 
                                                named_values_stream << setw(15) << input_.Names().at(EachItem) << " : " << input_.NamedValues().at(EachItem) << '\n';
                                            EndFor 
                                            
                                            return 
                                                "Vector\n" + 
                                                    VisualFormat(input_.Vector())+ 
                                                "Named Values\n"+ 
                                                    Indent(named_values_stream.str());
                                        }
                                    // just output vector 
                                    else if ( input_.Vector().size() >= 1 ) 
                                        {
                                            return "List\n" + VisualFormat(input_.Vector());
                                        }
                                    // just output named values 
                                    else if ( input_.Names().size() >= 1 )
                                        {
                                            stringstream output_stream;
                                            For_ input_.Names() _Vector 
                                                output_stream << setw(input_.SizeOfLargestName()) << input_.Names().at(EachItem) << " : " << input_.NamedValues().at(EachItem) << '\n';
                                            EndFor 
                                            return "List\n" + Indent(output_stream.str());
                                        }
                                    else 
                                        // output None  
                                        return "None\n";
                                }     
                    // << >>       
                        template <typename ITEM> ostream& operator<<(ostream& out, const List<ITEM>& input_)
                            {
                                return output(out, input_);
                            } 
                        template <typename ITEM> istream& operator>>(istream& in,        List<ITEM>& input_)
                            {
                                return input(in, input_);
                            }
                    // +           
                        template <class ITEM, class ANYTYPE>
                        List<ITEM>  operator+( List<ITEM>&     list_   , const ANYTYPE&   input_ )
                            { 
                                    // if it's not a list, just add it 
                                    if (ITEM(input_).Type() != "List")
                                        {
                                            list_.add(input_);
                                            return list_;
                                        }
                                    // if it is a list, then
                                    else 
                                        {
                                            // create a new list
                                            List<ITEM> combined_list;
                                            List<ITEM> list_2 = ITEM(input_);
                                            combined_list.Vector() = list_.Vector() + list_2.Vector();
                                                // if one of the list has named items 
                                                if ( list_.Names().size() != 0 || list_2.Names().size() != 0 )
                                                    {
                                                        // FIXME, optimize this a bit better  
                                                        vector <string> names      = ( list_.Names()       + list_2.Names()       );
                                                        vector <ITEM> named_values = ( list_.NamedValues() + list_2.NamedValues() );
                                                        For_  names  _Vector
                                                            combined_list[names.at(EachItem)] = named_values.at(EachItem);
                                                        EndFor 
                                                    }
                                            return combined_list;
                                        }

                                    
                            }
                    // ==          
                        template <class ANYTYPE> bool operator== ( const List<ANYTYPE>& input_1 , const List<ANYTYPE>& input_2)
                            {
                                if ( input_1.Data() == input_2.Data() && input_1.Names() == input_2.Names() )
                                    return true;
                                return false;
                            }
                    // Is__In__    
                        template <class ANYTYPE, class ANYSECONDTYPE> bool Is__In__(ANYTYPE input, List<ANYSECONDTYPE> input_list)
                            {
                                if ( input_list.Is__AName(input) )
                                    return true;
                                if ( input_list.Is__AValue(input) )
                                    return true;
                                return false;
                            }


            // make it so that List is always List<Item>
            #define List List<Item>







//////////////////////////////
//
// Item
//
//////////////////////////////
    class  Item 
        {
            // data
                string data; // later turn both of these into some kind of binary
                string type; 

        public:

            // constructors   
                    Item (                           ) { data =  "None"         ; type = "None"   ;}
                    Item ( string input_string       ) { data =  input_string   ; type = "string" ;}
                    Item ( const char input_string[] ) { data =  input_string   ; type = "string" ;}
                    Item ( int    input_         ) 
                        { 
                            stringstream input_method;
                            input_method << input_ ;
                            data = input_method.str(); 
                            type = "double" ;
                        }
                    Item ( double input_         ) 
                        { 
                            stringstream input_method;
                            // precision should max out before 20
                            input_method << setprecision(20) << input_ ;
                            data = input_method.str(); 
                            type = "double" ;
                        }
                    Item ( List   input_         ) 
                            { 
                                stringstream data_stream;
                                output(data_stream, input_);
                                // get the name
                                type = EverythingUpTo('\n', data_stream);
                                type = type.substr(0,type.size()-1);
                                // get the content
                                string content = Input_Unindent(data_stream);
                                // remove the closing newline and assign the data
                                data = content.substr(0,content.size()-1);
                            }
                    // generic constructor 
                    template<class ANYTYPE> explicit Item ( ANYTYPE input )
                            { 
                                stringstream data_stream;
                                output(data_stream, input);
                                // get the name
                                type = EverythingUpTo('\n', data_stream);
                                type = type.substr(0,type.size()-1);
                                // get the content
                                string content = Input_Unindent(data_stream);
                                // remove the closing newline
                                content = content.substr(0,content.size()-1);
                                data = content;
                                if (type == "double" || type == "integer")
                                    type = "double"; 
                            }
            // methods        
                    string Type()        const { return type ; }
                    string Data()        const { return data ; }
                    string Remove0s   () const      
                        {
                            If stoi(data) == stold(data) Then
                                return to_string(stoi(data));
                            IfNot
                                //FIXME, change this to remove trailing 0's 
                                return data;
                            EndIf 
                        } 
                    istream& Input  (istream& in)
                        {
                            // check name of type 
                            if ( EverythingUpTo('\n',in) != "Item\n")
                                return StreamFailed(in);
                            
                      
                                    // unindent members
                                    string content = Input_Unindent(in);
                                    // check fail
                                    if (content == "")
                                        return StreamFailed(in);
                                    // check end 
                                    if (content[content.size()-1] != '\n')
                                        return StreamFailed(in);
                                    // setup vars
                                    char should_always_be_a_newline;
                                    stringstream transfer_method;
                                    // get rid of the class's newline
                                    transfer_method << content.substr(0,content.size()-1);
                                
                        
                            // create empty versions of all data members 
                            input( transfer_method, data ); transfer_method.get(should_always_be_a_newline);
                            input( transfer_method, type ); transfer_method.get(should_always_be_a_newline);
                            
                      
                                    // if stream fails, then return fail 
                                    if (DidStreamFail(transfer_method))
                                        return StreamFailed(in);
                        
                        
                                    // if everything worked return the stream! 
                                    return in;
                        }  
            // operators      
                // =      
                    Item& operator=( const Item&  input_item    ) { data = input_item.Data()         ; type = input_item.Type(); return *this; }
                    Item& operator=( const char   input_string[]) { data = input_string              ; type = "string"         ; return *this; }
                    Item& operator=( const string input_string  ) { data = input_string              ; type = "string"         ; return *this; }
                    Item& operator=( const int    the_inputeger ) { data = to_string(the_inputeger)  ; type = "double"         ; return *this; }
                    Item& operator=( const float  input_        ) 
                        { 
                            stringstream input_method;
                            // precision should max out before 20
                            input_method << setprecision(20) << input_ ;
                            data = input_method.str(); 
                            type = "double"; 
                            return *this; 
                        }
                    Item& operator=( const double input_        ) 
                        { 
                            stringstream input_method;
                            // precision should max out before 20
                            input_method << setprecision(20) << input_ ;
                            data = input_method.str();
                            type = "double"; 
                            return *this; 
                        }
                    Item& operator=( const List   input_        ) 
                        { 
                            stringstream input_method;
                            output(input_method , input_ );
                            type = EverythingUpTo('\n',input_method);
                            type = type.substr(0,type.size()-1); 
                            data = Input_Unindent( input_method );
                            return *this; 
                        }
                    template<class ANYTYPE> Item& operator=( ANYTYPE input_ ) 
                        { 
                            stringstream input_method;
                            output(input_method , input_ );
                            type = EverythingUpTo('\n',input_method);
                            type = type.substr(0,type.size()-1); 
                            if (type == "default IO")
                                type = "Unknown";
                            data = Input_Unindent( input_method );
                            return *this; 
                        }
                // implicit conversion 
                    operator string() const 
                        {
                                return data;
                        }
                    operator double() const 
                        {
                            If type == "double" Then
                                return stold(data);
                            IfNot
                                Error("Somewhere there is an item thats not a number,\nand something is trying to make it a double\nthe item is "+data);
                            EndIf 
                        } 
                // []'s
                    // FIXME, right now you can get an element from a list inside an Item 
                    // but you can't assign anything to it, because behind-the-scenes it's just a string
                    // this will require changing the whole Item type to use pointers and dynamic memory 
                    // in order to fix
                    template <class ANYTYPE> Item operator[] (ANYTYPE input_) 
                        {
                            
                            // if its a list
                            if (this->Type() == "List")
                                {
                                    stringstream list_data;
                                    list_data << "List\n";
                                    list_data <<  Indent( this->Data() ) << '\n';
                                    List output_list;
                                    input(list_data, output_list);
                                    return output_list[input_];
                                }
                            else if (this->Type() == "string")
                                {
                                    return this->Data()[input_];
                                }
                            cout << this->Type() << '\n';
                            error("I think you're trying to use [] on a non-List non-String Item");
                            return Item();
                        }
        };
        // IO  
            ostream& output (ostream& out, const Item& input_)
                {
                    // name of class 
                    out << "Item" << '\n';
                    
                            stringstream data_members;
                            
                    // for each datamember
                    output( data_members , input_.Data()); data_members << '\n';
                    output( data_members , input_.Type()); data_members << '\n';

                            // format and return stream
                            out << Indent( data_members.str() );
                            return out;
                }
            istream& input  (istream& in,        Item& input_)
                {
                    return input_.Input(in);
                }  
            string   AsData (const Item& input_)
                {
                    stringstream out;
                    // name of class 
                    out << input_.Type() << '\n';
                    
                    stringstream data_members;
                            
                    // for each datamember
                    data_members << input_.Data() << '\n';

                    // format and return stream
                    out << Indent( data_members.str() );
                    return out.str();
                }
        // VisualFormat
            string VisualFormat( const Item& input_ )
                {
                    // number
                    if (input_.Type() == "double")
                        return input_.Data();
                    // list 
                    if (input_.Type() == "List")
                        {
                            // create a blank list 
                            List blank_list;
                            // put the data into a stream
                            stringstream data_stream;
                            data_stream << AsData(input_);
                            // populate the list 
                            input(data_stream, blank_list);
                            // output list using it's own stream operator
                            return VisualFormat(blank_list);
                        }
                    // other
                    return input_.Data();
                }
        // overloads
                // << >>   
                    ostream& operator<<(ostream& out, const Item& input_)
                        {
                            return output(out ,input_);
                        }
                    istream& operator>>(istream& in,        Item& input_)
                        {
                            return input(in,input_);
                        }                  
                // +    
                    Item  operator+( const char     the_input[] , const Item&   input_item  )
                        {
                            If input_item.Type() == "string" Then 
                                return Item(input_item.Data() + the_input);
                            IfNot 
                                Error("Trying to add a non-string Item: "+input_item.Data()+" to a string: " + the_input);
                            EndIf
                        }                 
                    Item  operator+( const int&     the_input   , const Item&   input_item  )
                        { 
                            if (input_item.Type() == "double")
                                return Item(stod(input_item.Data()) + the_input);
                            else
                                Error("Trying to add an int: "+to_string(input_item)+ " to non-number Item "+ input_item.Data());
                        }
                    Item  operator+( const double&  the_input   , const Item&   input_item  )
                        { 
                            if (input_item.Type() == "double")
                                return Item(stod(input_item.Data()) + the_input);
                            else
                                Error("Trying to add a double: "+to_string(input_item)+ " to non-number Item "+ input_item.Data());
                        }
                    Item  operator+( const string&  the_input   , const Item&   input_item  )
                        { 
                                If input_item.Type() == "double" Then
                                    return Item(the_input + input_item.Remove0s());
                                IfNot 
                                    return Item(the_input + input_item.Data());
                                EndIf 
                        } 
                    Item  operator+( const Item&    input_item  , const char    the_input[] )
                        {
                            If input_item.Type() == "string" Then 
                                return Item(input_item.Data() + the_input);
                            IfNot 
                                Error("Trying to add a non-string Item: "+input_item.Data()+" to a string: " + the_input);
                            EndIf
                        }
                    Item  operator+( const Item&    input_item  , const int&    the_input   )
                        { 
                            if (input_item.Type() == "double")
                                return Item(stod(input_item.Data()) + the_input);
                            else
                                Error("Trying to add an int: "+to_string(input_item)+ " to a non-number Item "+ input_item.Data());
                        }
                    Item  operator+( const Item&    input_item  , const double& the_input   )
                        { 
                            if (input_item.Type() == "double")
                                return Item(Item(stod(input_item.Data()) + the_input));
                            else
                                Error("Trying to add a double: "+to_string(input_item)+ " to a non-number Item "+ input_item.Data());
                        }
                    Item  operator+( const Item&    input_item  , const string& the_input   )
                        { 
                                If input_item.Type() == "double" Then
                                    return Item(input_item.Remove0s() + the_input);
                                IfNot 
                                    return Item(input_item.Data() + the_input);
                                EndIf 
                        } 
                    Item  operator+( const Item&    input_item1 , const Item&   input_item2 )
                        {
                            If input_item1.Type() == "double" && input_item2.Type() == "double" Then
                                return Item(stold(input_item1.Data()) + stold(input_item2.Data()));
                            IfNot
                                return Item(input_item1.Data() + input_item2.Data());
                            EndIf 
                        }
                // -    
                    Item  operator-( const int&     the_input   , const Item&   input_item  )
                        { 
                            if (input_item.Type() == "double")
                                return Item( the_input - stod(input_item.Data()));
                            else
                                Error("Trying to subtract an int: "+to_string(input_item)+ " from a non-number Item "+ input_item.Data());
                        }
                    Item  operator-( const double&  the_input   , const Item&   input_item  )
                        { 
                            if (input_item.Type() == "double")
                                return Item(the_input - stod(input_item.Data()));
                            else
                                Error("Trying to subtract a double: "+to_string(input_item)+ " from a non-number Item "+ input_item.Data());
                        }
                    Item  operator-( const Item&    input_item  , const int&    the_input   )
                        { 
                            if (input_item.Type() == "double")
                                return Item(stod(input_item.Data()) - the_input);
                            else
                                Error("Trying to subtract a non-number Item: "+input_item.Data()+ " from an int: "+to_string(input_item));
                        }
                    Item  operator-( const Item&    input_item  , const double& the_input   )
                        { 
                            if (input_item.Type() == "double")
                                return Item(the_input - stod(input_item.Data()));
                            else
                                Error("Trying to subtract a non-number Item: "+input_item.Data()+ " from a double "+to_string(input_item));
                        }
                    Item  operator-( const Item&    input_item1 , const Item&   input_item2 )
                        {
                            If input_item1.Type() == "double" && input_item2.Type() == "double" Then
                                return Item(stold(input_item1.Data()) - stold(input_item2.Data()));
                            IfNot
                                Error(string("I think somewhere in the code there is one item subtracting another")
                                    +"\nbut they are not both numbers"
                                    +"\nthe items are "
                                    +input_item1.Data()
                                    +" and "
                                    +input_item2.Data()
                                    +"\n");
                            EndIf 
                        }
                // *    
                    Item  operator*( const string&  the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "string"  Then 
                                Error("Somewhere there is a string Item: "+input_item.Data()+" being multipled by a string");
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                int number_ = stoi(input_item.Data());
                                If number_ == 0 Then 
                                    return Item(string(""));
                                IfNotThenIf number_ > 0 Then 
                                    string output_string; 
                                    Loop_ number_ _Times 
                                        output_string = output_string + the_input;
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            EndIf
                        }  
                    Item  operator*( const int&     the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "string"  Then 
                                If the_input <= 0 Then 
                                    return Item(string(""));
                                IfNotThenIf the_input > 0 Then 
                                    string output_string; 
                                    Loop_ the_input _Times 
                                        output_string = output_string + input_item.Data();
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                return Item(the_input * stold(input_item.Data()));
                            EndIf
                        }  
                    Item  operator*( const double&  the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "string"  Then 
                                int number_ = static_cast<int>(  the_input  );
                                If number_ == 0 Then 
                                    return Item(string(""));
                                IfNotThenIf number_ > 0 Then 
                                    string output_string; 
                                    Loop_ number_ _Times 
                                        output_string = output_string + input_item.Data();
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                return Item(the_input * stold(input_item.Data()));
                            EndIf
                        }  
                    Item  operator*( const char&    the_input   , const Item&   input_item  )
                        {
                            return Item(CharToString(the_input) * input_item);
                        }
                    Item  operator*( const Item&    input_item  , const double& the_input   )
                        { 
                            If input_item.Type() == "string"  Then 
                                int number_ = static_cast<int>(  the_input  );
                                If number_ == 0 Then 
                                    return Item(string(""));
                                IfNotThenIf number_ > 0 Then 
                                    string output_string; 
                                    Loop_ number_ _Times 
                                        output_string = output_string + input_item.Data();
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                return Item(the_input * stold(input_item.Data()));
                            EndIf
                        }                  
                    Item  operator*( const Item&    input_item  , const int&    the_input   )
                        { 
                            If input_item.Type() == "string"  Then 
                                int number_ = static_cast<int>(  the_input  );
                                If number_ == 0 Then 
                                    return Item(string(""));
                                IfNotThenIf number_ > 0 Then 
                                    string output_string; 
                                    Loop_ number_ _Times 
                                        output_string = output_string + input_item.Data();
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                return Item(the_input * stold(input_item.Data()));
                            EndIf
                        }  
                    Item  operator*( const Item&    input_item  , const string& the_input   )
                        { 
                            If input_item.Type() == "string"  Then 
                                Error("Somewhere there is a string Item being multipled by a string");
                            IfNotThenIf  input_item.Type() == "double"  Then 
                                int number_ = stoi(input_item.Data());
                                If number_ == 0 Then 
                                    return Item(string(""));
                                IfNotThenIf number_ > 0 Then 
                                    string output_string; 
                                    Loop_ number_ _Times 
                                        output_string = output_string + the_input;
                                    EndLoop__Times 
                                    return Item(output_string);
                                IfNot 
                                    Error("Somewhere there is a string be multipled by a negative Item");
                                EndIf 
                            EndIf
                        }  
                    Item  operator*( const Item&    input_item  , const char&   the_input   )
                        {
                            return Item(input_item * CharToString(the_input) );
                        }
                    Item  operator*( const Item&    input_item1 , const Item&   input_item2 )
                        {
                            If ( input_item1.Type() == "double" && input_item2.Type() == "double" )
                                Then
                                    return Item(stod(input_item1.Data()) * stod(input_item2.Data()));
                            IfNotThenIf ( input_item1.Type() == "string" && input_item2.Type() == "double" ) 
                                Then 
                                    return Item(input_item1 * stod(input_item2.Data()));

                            IfNotThenIf( input_item1.Type() == "double" && input_item2.Type() == "string" )
                                Then
                                    return Item(stod(input_item1.Data()) * input_item2);
                            IfNot
                                Error(string("I think somewhere in the code there is one item multipling another")
                                    +"\nbut neither of them are numbers"
                                    +"\nthe items are "
                                    +input_item1.Data() + " type: " + input_item1.Type()
                                    +" and "
                                    +input_item2.Data() + " type: " + input_item2.Type()
                                    +"\n");
                            EndIf 
                        }
                // /    
                    Item  operator/( const Item&    input_item  , const int&    the_input   )
                        { 
                            If input_item.Type() == "double"  Then 
                                If the_input ==0 Then 
                                    Error("Division by 0 somewhere");
                                EndIf
                                return Item(stold( input_item.Data() )  / the_input);
                            IfNot 
                                Error("Trying to divide something from an item thats not a number");
                            EndIf
                        }
                    Item  operator/( const Item&    input_item  , const double& the_input   )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(stold( input_item.Data() ) / the_input);
                            IfNot 
                                Error("Trying to divide something from an item thats not a number");
                            EndIf
                        }
                    Item  operator/( const int&     the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "double" Then
                                If     stod( input_item.Data() ) <  0.00000000001 
                                    && stod( input_item.Data() ) > -0.00000000001 
                                Then 
                                    Error("Division by 0 somewhere");
                                IfNot
                                    return Item(the_input / stold( input_item.Data() ));
                                EndIf 
                            IfNot 
                                Error("Trying to divide something from an item thats not a number");
                            EndIf
                        }
                    Item  operator/( const double&  the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "double" Then
                                If     stod( input_item.Data() ) <  0.00000000001 
                                    && stod( input_item.Data() ) > -0.00000000001 
                                Then 
                                    Error("Division by 0 somewhere");
                                IfNot
                                    return Item(the_input / stold( input_item.Data() ));
                                EndIf 
                            IfNot 
                                Error("Trying to divide something from an item thats not a number");
                            EndIf
                        }
                    Item  operator/( const Item&    input_item1 , const Item&   input_item2 )
                        {
                            If input_item1.Type() == "double" && input_item2.Type() == "double" Then
                                return Item(stold(input_item1.Data()) / stold(input_item2.Data()));
                            IfNot
                                Error(string("I think somewhere in the code there is one item dividing another")
                                    +"\nbut they are not both numbers"
                                    +"\nthe items are "
                                    +input_item1.Data()
                                    +" and "
                                    +input_item2.Data()
                                    +"\n");
                            EndIf 
                        }
                // ^    
                    Item  operator^( const Item&    input_item  , const int&    the_input   )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow(  stold( input_item.Data() ) , the_input));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const Item&    input_item  , const float&  the_input   )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow(  stold( input_item.Data() ) , the_input));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const Item&    input_item  , const double& the_input   )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow(  stold( input_item.Data() ) , the_input));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const int&     the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow( the_input, stold( input_item.Data() )));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const float&   the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow( the_input, stold( input_item.Data() )));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const double&  the_input   , const Item&   input_item  )
                        { 
                            If input_item.Type() == "double"  Then 
                                return Item(pow( the_input, stold( input_item.Data() )));
                            IfNot 
                                Error("Trying to use an exponent with something from an item thats not a number");
                            EndIf
                        }
                    Item  operator^( const Item&    input_item1 , const Item&   input_item2 )
                        {
                            If input_item1.Type() == "double" && input_item2.Type() == "double" Then
                                return Item(stod(input_item1.Data()) ^ input_item2);
                            IfNot
                                Error(string("I think somewhere in the code there is one item to the power of another")
                                    +"\nbut they are not both numbers"
                                    +"\nthe items are "
                                    +input_item1.Data()
                                    +" and "
                                    +input_item2.Data()
                                    +"\n");
                            EndIf 
                        }
                // ==   
                    bool operator== ( const Item& input_1 , const Item& input_2)
                        {
                            if ( input_1.Data() == input_2.Data() && input_1.Type() == input_2.Type() )
                                return true;
                            return false;
                        }
                //  Function overloads
                    string to_string(const Item& input_item)
                        { 
                            return input_item.Data();
                        } 
                    

        // the 'var' class is actually the Item class, I just use #define to change it to var
        #define var Item






//////////////////////////////
//
// General functions
//
//////////////////////////////
    // Type()
            template <class ANYTYPE>
            string Type(ANYTYPE input)
                {
                    return Item(input).Type();
                }
            string Type(int input)
                {
                    return "int";
                }
            string Type(double input)
                {
                    return "double";
                }
            string Type(char input)
                {
                    return "char";
                }
    // len()       
            template <class ANYTYPE>
            long int len(ANYTYPE input_)
                {
                    return input_.size();
                }






