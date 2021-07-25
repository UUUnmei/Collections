#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include"symbolTable.h"
#include"Parser.h"

using namespace std;

int scope_level = 0;
int scope_count = 0;
string cvt_name(const string& s, int level, int count) {
    return s + "_main_" + to_string(level) + "_" + to_string(count);
}
// 进作用域
void Parser::INSCP(void) {
    SymbolTable.inc_scope(scope_level, scope_count);
}
// 出作用域
void Parser::OTSCP(void) {
    SymbolTable.dec_scope(scope_level, scope_count);
}

void Parser::err(const string&s) {
    string t = "第" + to_string(PresentToken._row) + "行错误" + s;
    cout << t << '\n';
    if (debug) {
        outputfile << t << '\n';
    }
    exit(1);
}
void Parser::log(const string& s) {
    if (debug) {
        outputfile << s << '\n';
    }
}

void Parser::Next()
{
    log(PresentToken._value);
    //outputfile << PresentToken._value << '\n';
    this->Grammer_Lexer.next_word(this->PresentToken);

    return;
}

Parser::Parser(string source_file)
{
    Grammer_Lexer.setSourceFile(source_file);
    this->global_type = 0;
    this->global_name = "";
    this->const_count = 0;
    this->offset = 0;
}


void Parser::Program_sp()//sp——SubProgram 子程序简写，这里是“程序”的递归子程序
{

    Next();
    MainFunc_sp();

    //********************************
    //GENL
    //********************************

    if (PresentToken._value == "#")
    {
        log( "程序" );
        log( "编译成功！" );
        cout << "编译成功！\n";
        return;
    }
    else
    {
        err("error!");
    }
}

void Parser::MainFunc_sp()//sp——SubProgram 子程序简写，这里是“主函数”的递归子程序
{

    if (PresentToken._value != "void")
    {
        err("主函数类型为  void !");
    }
    Next();
    if (PresentToken._value != "main")
    {
        err("主函数函数名为 main ！");
    }
    Next();
    if (PresentToken._value != "(")
    {
        err("主函数函数名后要跟 ( ！");
    }
    Next();
    if (PresentToken._value != ")")
    {
        err("( ) 不匹配！");

    }
    Next();

    //********************************
    //
    //INIT
    SymbolTable.initialization();
    SymbolTable.ini_scopes();
    scope_level = scope_count = 0;
    //********************************
    if (PresentToken._value != "{")
    {
        err("程序段以 { 开始！");
        
    }
    Next();
    CompoundStatement_sp();
    if (PresentToken._value != "}")
    {
        err("程序段以 } 结束！");
        
    }
    Next();
    log( "主函数" );
}

void Parser::CompoundStatement_sp()//sp——SubProgram 子程序简写，这里是“复合语句”的递归子程序
{
    if (PresentToken._value == "int" || PresentToken._value == "double")
    {
        VariableDeclaration_sp();
    }
    StatementColumn_sp();
    log( "复合语句" );
}

void Parser::VariableDeclaration_sp()//sp——SubProgram 子程序简写，这里是“变量说明”的递归子程序
{
i:
    VariableDefinition_sp();
    if (PresentToken._value == ";")
    {
        Next();
        if (PresentToken._value != "int" && PresentToken._value != "double")
        {
            log( "变量说明" );
            return;
        }
        else
        {
            goto i;   
        }
    }
    else
    {
        err("缺少 ; ！");
        
    }
    //log( "变量说明" );
}

void Parser::VariableDefinition_sp()//sp——SubProgram 子程序简写，这里是“变量定义”的递归子程序
{

    if (PresentToken._value == "int")
    {
    i1:
        Next();
        //FILNT
        this->global_type = 1;
        if (PresentToken._type == TokenType::Identifier)
        {
            //FILPT
            RepeatStatement_ERROR();
            this->SymbolTable.insert_symbol(this->PresentToken._value, this->global_type, cattType::VAR);
            string t = PresentToken._value;
            Next();
            if (PresentToken._type == TokenType::Assign) {
                Next();
                Expression_sp();
                ir.add_quad(Opcode::Assign, SEM[SEM.size() - 1]._value, t);
                SEM.pop_back();
                log( "赋值语句" );
            }

            if (PresentToken._value == ",")
            {
                goto i1;
            }
            else
            {
                log( "变量定义" );
                return;
            }
        }
        else
        {
            err("标识符错误！");
            
        }
    }
    //else if (PresentToken._value == "double")
    //{

    //i2:
    //    Next();
    //    //FILNT
    //    this->global_type = 2;
    //    if (PresentToken._type == TokenType::Identifier)
    //    {
    //        //FILPT
    //        //RepeatStatement_ERROR();
    //        this->SymbolTable.insert_symbol(this->PresentToken._value, this->global_type, cattType::VAR);

    //        Next();
    //        if (PresentToken._value == ",")
    //        {
    //            goto i2;
    //        }
    //        else
    //        {
    //            log( "变量定义" );
    //            return;
    //        }
    //    }
    //    else
    //    {
    //        err("标识符错误！");
    //        
    //    }
    //}
    else
    {
        err("int or double 错误！");
        
    }
    //log( "变量定义" );
}

void Parser::StatementColumn_sp()//sp——SubProgram 子程序简写，这里是“语句列”的递归子程序
{
i:
    if (PresentToken._value == "{" || PresentToken._type == TokenType::Identifier || PresentToken._value == ";"
        || PresentToken._type == TokenType::IF 
        || PresentToken._type == TokenType::WHILE
        || PresentToken._type == TokenType::DO
        || PresentToken._type == TokenType::FOR
        || PresentToken._type == TokenType::INT
        || PresentToken._type == TokenType::DOUBLE
        || PresentToken._type == TokenType::SCANF
        || PresentToken._type == TokenType::PRINTF)
    {
        Statement_sp();
        goto i;
    }
    log( "语句列" );
}

void Parser::Statement_sp()//sp——SubProgram 子程序简写，这里是“语句”的递归子程序
{
    if (PresentToken._value == "{")
    {
        Next();
        StatementColumn_sp();
        if (PresentToken._value == "}")
        {
            log( "语句" );
            Next();
            return;
        }
        else
        {

            err("语句应以 } 结束！");
            
        }
    }
    else if (PresentToken._type == TokenType::Identifier)
    {
        AssignmentStatement_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "语句" );      
            return;
        }
        else
        {
            err("缺少 ; ！");
            
        }
    }
    else if (PresentToken._type == TokenType::IF) {
        ConditionStatement_sp();
        log( "语句" );
    }
    else if (PresentToken._type == TokenType::WHILE
        || PresentToken._type == TokenType::DO
        || PresentToken._type == TokenType::FOR) {
        LoopStatement_sp();
        log( "语句" );
    }
    else if (PresentToken._type == TokenType::INT
        || PresentToken._type == TokenType::DOUBLE) {
        VariableDeclaration_sp();
        log( "语句" );
    }
    else if (PresentToken._type == TokenType::SCANF) {
        Scanf_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "语句" );     
            return;
        }
        else
        {
            err("缺少 ; ！");
            
        }
    }
    else if (PresentToken._type == TokenType::PRINTF) {
        Printf_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "语句" );
            return;
        }
        else
        {
            err("缺少 ; ！");
            
        }
    }
    else
    {
        if (PresentToken._value == ";")
        {
            Next();
            log( "语句" ); 
            return;
        }
        else
        {
            err("缺少 ; ！");
            
        }
    }
}

void Parser::AssignmentStatement_sp()//sp——SubProgram 子程序简写，这里是“赋值语句”的递归子程序
{
    if (PresentToken._type == TokenType::Identifier)
    {
        //PUSH(v)
        UnStated_ERROR();
        this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), PresentToken._value);
        SEM.push_back(PresentToken);
        Next();
        if (PresentToken._value == "=")
        {
            Next();
            Expression_sp();
            //ASSI(=)
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Assign, SEM[SEM.size() - 1]._value, SEM[SEM.size() - 2]._value);
            SEM.pop_back();
            SEM.pop_back();
            log( "赋值语句" );
            return;
        }
        else
        {
            err(" = 错误！");
            
        }
    }
    else
    {
        err("标识符错误！");
        
    }
}

void Parser::Expression_sp()//sp——SubProgram 子程序简写，这里是“表达式”的递归子程序
{
    //if (PresentToken._value == "+" || PresentToken._value == "-")
    //{
    //    Next();
    //}
    Item_sp();
    while (1) {
        if (PresentToken._type == TokenType::Add) {
            AddingOperator_sp();
            Item_sp();
            Token t;
            // new t
            this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type) == TokenType::INT ? 1 : 2, t._value);
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Add, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
            SEM.pop_back();
            SEM.pop_back();
            SEM.push_back(t);
        }
        else if (PresentToken._type == TokenType::Sub) {
            AddingOperator_sp();
            Item_sp();

            Token t;
            // new t
            this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type) == TokenType::INT ? 1 : 2, t._value);
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Sub, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
            SEM.pop_back();
            SEM.pop_back();
            SEM.push_back(t);
        }
        else {
            break;
        }
    }
    log( "表达式" );
}

void Parser::Item_sp()//sp——SubProgram 子程序简写，这里是“项”的递归子程序
{
    Factor_sp();

    while (1) {
        if (PresentToken._type == TokenType::Mul) {
            MultiplyingOperator_sp();
            Factor_sp();

            Token t;
            // new t
            this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type) == TokenType::INT ? 1 : 2, t._value);
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Mul, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
            SEM.pop_back();
            SEM.pop_back();
            SEM.push_back(t);
        }
        else if (PresentToken._type == TokenType::Div) {
            MultiplyingOperator_sp();
            Factor_sp();

            Token t;
            // new t
            this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type) == TokenType::INT ? 1 : 2, t._value);
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Div, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
            SEM.pop_back();
            SEM.pop_back();
            SEM.push_back(t);
        }
        else if (PresentToken._type == TokenType::Mod) {
            MultiplyingOperator_sp();
            Factor_sp();

            Token t;
            // new t
            this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type) == TokenType::INT ? 1 : 2, t._value);
            //Double_UnStated_ERROR();
            ir.add_quad(Opcode::Mod, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
            SEM.pop_back();
            SEM.pop_back();
            SEM.push_back(t);
        }
        else {
            break;
        }
    }
    log( "项" );
}

void Parser::Factor_sp()//sp——SubProgram 子程序简写，这里是“因子”的递归子程序
{
    if (PresentToken._type == TokenType::Identifier)
    {
        //PUSH(v)
        UnStated_ERROR();
        this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), PresentToken._value);
        SEM.push_back(PresentToken);
        Next();

    }
    else if (PresentToken._value == "(")
    {
        Next();
        Expression_sp();
        if (PresentToken._value == ")")
        {
            Next();
            log( "因子" );
            return;
        }
        else
        {
            err("()不匹配！");
            
        }
    }
    else
    {
        Number_sp();
        //PUSH(v)
    }
    log( "因子" );
}

void Parser::AddingOperator_sp()//sp——SubProgram 子程序简写，这里是“加法运算符”的递归子程序
{
    if (PresentToken._value == "+" || PresentToken._value == "-")
    {
        Next();
        log( "加法运算符" ); 
    }
    else
    {
        err("+ -错误！");
        
    }
    
}

void Parser::MultiplyingOperator_sp()//sp——SubProgram 子程序简写，这里是“乘法运算符”的递归子程序
{
    if (PresentToken._value == "*" || PresentToken._value == "/" || PresentToken._value == "%")
    { 
        Next();
        log( "乘法运算符" );
       
    }
    else
    {
        err("* / %错误！");
        
    }
    
}

void Parser::Number_sp()//sp——SubProgram 子程序简写，这里是“数”的递归子程序
{
    bool neg = false;
    if (PresentToken._type == TokenType::Add || PresentToken._type == TokenType::Sub)
    {
        
        // 如果是负号应该把接下来读入的数字取负值
        if (PresentToken._type == TokenType::Sub) {
            neg = true;
        }

        Next();
        
    }

    if (PresentToken._type == TokenType::Double || PresentToken._type == TokenType::Integer)
    {
        //PUSH(v)
        // 下面两行不对劲。。立即数怎么还报未定义，
        //UnStated_ERROR();
        //this->SymbolTable.serach_symbol(PresentToken._value, PresentToken._value);
        

        //if (PresentToken._type == TokenType::Double) {
        //    SymbolTable.insert_const(2, stod(PresentToken._value) * (neg ? -1 : 1));
        //}
        //else {
        //    SymbolTable.insert_const(1, stoi(PresentToken._value) * (neg ? -1 : 1));
        //}
        

        //立即数不用进表了，不改名
        PresentToken._value = (neg ? "-" : "") + PresentToken._value;
        SEM.push_back(PresentToken);
        Next();
        log( "数" );
        
    }
    else
    {
        err("数值类型错误！");
        
    }
    
}


void Parser::RelationaOperator_sp()
{
    if (PresentToken._type == TokenType::LT || PresentToken._type == TokenType::LE
        || PresentToken._type == TokenType::GT || PresentToken._type == TokenType::GE
        || PresentToken._type == TokenType::NE || PresentToken._type == TokenType::EQ) {
        
        Next(); 
        log( "关系运算符" );
        
    }
    else {
        err("不是关系运算符！");
        
    }
}


void Parser::Condition_sp()
{
    Expression_sp();
    if (PresentToken._type == TokenType::LT || PresentToken._type == TokenType::LE
        || PresentToken._type == TokenType::GT || PresentToken._type == TokenType::GE
        || PresentToken._type == TokenType::NE || PresentToken._type == TokenType::EQ) {
        auto tp = PresentToken._type;
        RelationaOperator_sp();
        Expression_sp();
        Opcode new_tp;
        switch (tp) {
        case  TokenType::LT:
            new_tp = Opcode::LT;
            break;
        case  TokenType::LE:
            new_tp = Opcode::LE;
            break;
        case  TokenType::GT:
            new_tp = Opcode::GT;
            break;
        case  TokenType::GE:
            new_tp = Opcode::GE;
            break;
        case  TokenType::NE:
            new_tp = Opcode::NE;
            break;
        case  TokenType::EQ:
            new_tp = Opcode::EQ;
            break;
        }
        Token t;
        this->SymbolTable.new_temp(max(SEM[SEM.size() - 1]._type, SEM[SEM.size() - 2]._type)== TokenType::INT ? 1 : 2, t._value);
        ir.add_quad(new_tp, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
        SEM.pop_back();
        SEM.pop_back();
        SEM.push_back(t);
    }
    else {
        // 单个表达式时应按语法补全为 exp != 0的形式，方便处理

        // 临时弄个0
        SEM.emplace_back("0", 0, 0, TokenType::Integer);
        // 然后正常加四元式
        Token t;
        this->SymbolTable.new_temp(1, t._value);
        ir.add_quad(Opcode::NE, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
        SEM.pop_back();
        SEM.pop_back();
        SEM.push_back(t);
    }
    log( "条件" );
}

void Parser::ConditionStatement_sp()
{
    if(PresentToken._type != TokenType::IF){
        err("条件语句关键字为 if! ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("条件前应为 ( ");
        
    }
    Next();
    Condition_sp();
    if (PresentToken._type != TokenType::RightBracket) {
        err("条件后应为 ) ");
        
    }
    ir.add_quad(Opcode::IF, SEM.back()._value);
    SEM.pop_back();
    
    Next();
    // 进作用域
    INSCP();
    Statement_sp();
    // 出作用域
    OTSCP();
    if (PresentToken._type == TokenType::ELSE) {
        
        Next();
        ir.add_quad(Opcode::EL);
        // 进作用域
        INSCP();
        Statement_sp();
        // 出作用域
        OTSCP();
    }
    ir.add_quad(Opcode::IE);
    log( "条件语句" );
}
void Parser::LoopStatement_sp()
{
    if (PresentToken._type == TokenType::WHILE) {
        ir.add_quad(Opcode::WH);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("条件前应为 ( ");
            
        }
        Next();
        Condition_sp();
        if (PresentToken._type != TokenType::RightBracket) {
            err("条件后应为 ) ");
            
        }
        
        Next();
        ir.add_quad(Opcode::DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
        // 进作用域
        INSCP();
        Statement_sp();
        // 出作用域
        OTSCP();
        ir.add_quad(Opcode::WE);
    }
    else if (PresentToken._type == TokenType::DO) {
        ir.add_quad(Opcode::dw_LB);
        Next();
        // 进作用域
        INSCP();
        Statement_sp();
        // 出作用域
        OTSCP();
        if (PresentToken._type != TokenType::WHILE) {
            err("do-while语句缺少while ");
            
        }
        ir.add_quad(Opcode::dw_WH);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("条件前应为 ( ");
            
        }
        Next();
        Condition_sp();
        if (PresentToken._type != TokenType::RightBracket) {
            err("条件后应为 ) ");
            
        }
        Next();
        if (PresentToken._type != TokenType::SemiColon) {
            err("do-while语句后缺少 ; 分隔符 ");
            
        }
        Next();
        ir.add_quad(Opcode::dw_DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
    }
    else if (PresentToken._type == TokenType::FOR) {
        ir.add_quad(Opcode::fr_FR);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("for后面应有 ( ");
            
        }
        Next();
        AssignmentStatement_sp();
        if (PresentToken._type != TokenType::SemiColon) {
            err("赋值语句后缺少 ; 分隔符 ");
            
        }
        ir.add_quad(Opcode::fr_LB);
        Next();
        Condition_sp();
        ir.add_quad(Opcode::fr_DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
        if (PresentToken._type != TokenType::SemiColon) {
            err("条件后缺少 ; 分隔符 ");
            
        }
        Next();
        // 把更新语句的四元式放到循环体的四元式后面，方便
        auto p1 = ir.get_cur_quad_position();
        p1++;
        AssignmentStatement_sp();
        auto p2 = ir.get_cur_quad_position();
        if (PresentToken._type != TokenType::RightBracket) {
            err("缺少 ) ");
            
        }
        Next();

        // 进作用域
        INSCP();
        Statement_sp();
        // 出作用域
        OTSCP();
        ir.move_backward(p1, p2);
        ir.add_quad(Opcode::fr_FE);
    }
    else {
        err("，循环语句关键字错误 ");
        
    }
    log( "循环语句" );
}


void Parser::Scanf_sp() {
    if (PresentToken._type != TokenType::SCANF) {
        err("读语句scanf关键字错误 ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("scanf后应为 ( ");
        
    }
    Next();
    if (PresentToken._type != TokenType::Identifier) {
        err("标识符错误 ");
        
    }
    UnStated_ERROR();
    // 搜一下表，得用改后的名
    // 相当于赋值
    this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), PresentToken._value);
    ir.add_quad(Opcode::SCANF, PresentToken._value);
    Next();
    if (PresentToken._type != TokenType::RightBracket) {
        err("应有 )结束 ");
        
    }
    Next();
    log( "读语句" );
    
}

void Parser::Printf_sp() {
    if (PresentToken._type != TokenType::PRINTF) {
        err("写语句printf关键字错误 ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("printf后应为 ( ");
        
    }
    Next();
    Expression_sp();
    ir.add_quad(Opcode::PRINTF, SEM[SEM.size() - 1]._value);
    if (PresentToken._type != TokenType::RightBracket) {
        err("应有 )结束 ");
        
    }
    Next();
    log( "写语句" );
   
}




void Parser::UnStated_ERROR()
{
    string temp;
    if (this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), temp) == false)
    {
        err("标识符" + PresentToken._value + "未声明！");
    }
}




void Parser::RepeatStatement_ERROR()
{
    string temp; 
    string name = cvt_name(PresentToken._value, scope_level, scope_count);
    SymbolTable.serach_symbol(name, temp);
    if (temp == name)
    {
        err("标识符" + PresentToken._value + "重复声明！");
    }
}


