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
// ��������
void Parser::INSCP(void) {
    SymbolTable.inc_scope(scope_level, scope_count);
}
// ��������
void Parser::OTSCP(void) {
    SymbolTable.dec_scope(scope_level, scope_count);
}

void Parser::err(const string&s) {
    string t = "��" + to_string(PresentToken._row) + "�д���" + s;
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


void Parser::Program_sp()//sp����SubProgram �ӳ����д�������ǡ����򡱵ĵݹ��ӳ���
{

    Next();
    MainFunc_sp();

    //********************************
    //GENL
    //********************************

    if (PresentToken._value == "#")
    {
        log( "����" );
        log( "����ɹ���" );
        cout << "����ɹ���\n";
        return;
    }
    else
    {
        err("error!");
    }
}

void Parser::MainFunc_sp()//sp����SubProgram �ӳ����д�������ǡ����������ĵݹ��ӳ���
{

    if (PresentToken._value != "void")
    {
        err("����������Ϊ  void !");
    }
    Next();
    if (PresentToken._value != "main")
    {
        err("������������Ϊ main ��");
    }
    Next();
    if (PresentToken._value != "(")
    {
        err("��������������Ҫ�� ( ��");
    }
    Next();
    if (PresentToken._value != ")")
    {
        err("( ) ��ƥ�䣡");

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
        err("������� { ��ʼ��");
        
    }
    Next();
    CompoundStatement_sp();
    if (PresentToken._value != "}")
    {
        err("������� } ������");
        
    }
    Next();
    log( "������" );
}

void Parser::CompoundStatement_sp()//sp����SubProgram �ӳ����д�������ǡ�������䡱�ĵݹ��ӳ���
{
    if (PresentToken._value == "int" || PresentToken._value == "double")
    {
        VariableDeclaration_sp();
    }
    StatementColumn_sp();
    log( "�������" );
}

void Parser::VariableDeclaration_sp()//sp����SubProgram �ӳ����д�������ǡ�����˵�����ĵݹ��ӳ���
{
i:
    VariableDefinition_sp();
    if (PresentToken._value == ";")
    {
        Next();
        if (PresentToken._value != "int" && PresentToken._value != "double")
        {
            log( "����˵��" );
            return;
        }
        else
        {
            goto i;   
        }
    }
    else
    {
        err("ȱ�� ; ��");
        
    }
    //log( "����˵��" );
}

void Parser::VariableDefinition_sp()//sp����SubProgram �ӳ����д�������ǡ��������塱�ĵݹ��ӳ���
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
                log( "��ֵ���" );
            }

            if (PresentToken._value == ",")
            {
                goto i1;
            }
            else
            {
                log( "��������" );
                return;
            }
        }
        else
        {
            err("��ʶ������");
            
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
    //            log( "��������" );
    //            return;
    //        }
    //    }
    //    else
    //    {
    //        err("��ʶ������");
    //        
    //    }
    //}
    else
    {
        err("int or double ����");
        
    }
    //log( "��������" );
}

void Parser::StatementColumn_sp()//sp����SubProgram �ӳ����д�������ǡ�����С��ĵݹ��ӳ���
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
    log( "�����" );
}

void Parser::Statement_sp()//sp����SubProgram �ӳ����д�������ǡ���䡱�ĵݹ��ӳ���
{
    if (PresentToken._value == "{")
    {
        Next();
        StatementColumn_sp();
        if (PresentToken._value == "}")
        {
            log( "���" );
            Next();
            return;
        }
        else
        {

            err("���Ӧ�� } ������");
            
        }
    }
    else if (PresentToken._type == TokenType::Identifier)
    {
        AssignmentStatement_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "���" );      
            return;
        }
        else
        {
            err("ȱ�� ; ��");
            
        }
    }
    else if (PresentToken._type == TokenType::IF) {
        ConditionStatement_sp();
        log( "���" );
    }
    else if (PresentToken._type == TokenType::WHILE
        || PresentToken._type == TokenType::DO
        || PresentToken._type == TokenType::FOR) {
        LoopStatement_sp();
        log( "���" );
    }
    else if (PresentToken._type == TokenType::INT
        || PresentToken._type == TokenType::DOUBLE) {
        VariableDeclaration_sp();
        log( "���" );
    }
    else if (PresentToken._type == TokenType::SCANF) {
        Scanf_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "���" );     
            return;
        }
        else
        {
            err("ȱ�� ; ��");
            
        }
    }
    else if (PresentToken._type == TokenType::PRINTF) {
        Printf_sp();
        if (PresentToken._value == ";")
        {
            Next();
            log( "���" );
            return;
        }
        else
        {
            err("ȱ�� ; ��");
            
        }
    }
    else
    {
        if (PresentToken._value == ";")
        {
            Next();
            log( "���" ); 
            return;
        }
        else
        {
            err("ȱ�� ; ��");
            
        }
    }
}

void Parser::AssignmentStatement_sp()//sp����SubProgram �ӳ����д�������ǡ���ֵ��䡱�ĵݹ��ӳ���
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
            log( "��ֵ���" );
            return;
        }
        else
        {
            err(" = ����");
            
        }
    }
    else
    {
        err("��ʶ������");
        
    }
}

void Parser::Expression_sp()//sp����SubProgram �ӳ����д�������ǡ����ʽ���ĵݹ��ӳ���
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
    log( "���ʽ" );
}

void Parser::Item_sp()//sp����SubProgram �ӳ����д�������ǡ���ĵݹ��ӳ���
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
    log( "��" );
}

void Parser::Factor_sp()//sp����SubProgram �ӳ����д�������ǡ����ӡ��ĵݹ��ӳ���
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
            log( "����" );
            return;
        }
        else
        {
            err("()��ƥ�䣡");
            
        }
    }
    else
    {
        Number_sp();
        //PUSH(v)
    }
    log( "����" );
}

void Parser::AddingOperator_sp()//sp����SubProgram �ӳ����д�������ǡ��ӷ���������ĵݹ��ӳ���
{
    if (PresentToken._value == "+" || PresentToken._value == "-")
    {
        Next();
        log( "�ӷ������" ); 
    }
    else
    {
        err("+ -����");
        
    }
    
}

void Parser::MultiplyingOperator_sp()//sp����SubProgram �ӳ����д�������ǡ��˷���������ĵݹ��ӳ���
{
    if (PresentToken._value == "*" || PresentToken._value == "/" || PresentToken._value == "%")
    { 
        Next();
        log( "�˷������" );
       
    }
    else
    {
        err("* / %����");
        
    }
    
}

void Parser::Number_sp()//sp����SubProgram �ӳ����д�������ǡ������ĵݹ��ӳ���
{
    bool neg = false;
    if (PresentToken._type == TokenType::Add || PresentToken._type == TokenType::Sub)
    {
        
        // ����Ǹ���Ӧ�ðѽ��������������ȡ��ֵ
        if (PresentToken._type == TokenType::Sub) {
            neg = true;
        }

        Next();
        
    }

    if (PresentToken._type == TokenType::Double || PresentToken._type == TokenType::Integer)
    {
        //PUSH(v)
        // �������в��Ծ�������������ô����δ���壬
        //UnStated_ERROR();
        //this->SymbolTable.serach_symbol(PresentToken._value, PresentToken._value);
        

        //if (PresentToken._type == TokenType::Double) {
        //    SymbolTable.insert_const(2, stod(PresentToken._value) * (neg ? -1 : 1));
        //}
        //else {
        //    SymbolTable.insert_const(1, stoi(PresentToken._value) * (neg ? -1 : 1));
        //}
        

        //���������ý����ˣ�������
        PresentToken._value = (neg ? "-" : "") + PresentToken._value;
        SEM.push_back(PresentToken);
        Next();
        log( "��" );
        
    }
    else
    {
        err("��ֵ���ʹ���");
        
    }
    
}


void Parser::RelationaOperator_sp()
{
    if (PresentToken._type == TokenType::LT || PresentToken._type == TokenType::LE
        || PresentToken._type == TokenType::GT || PresentToken._type == TokenType::GE
        || PresentToken._type == TokenType::NE || PresentToken._type == TokenType::EQ) {
        
        Next(); 
        log( "��ϵ�����" );
        
    }
    else {
        err("���ǹ�ϵ�������");
        
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
        // �������ʽʱӦ���﷨��ȫΪ exp != 0����ʽ�����㴦��

        // ��ʱŪ��0
        SEM.emplace_back("0", 0, 0, TokenType::Integer);
        // Ȼ����������Ԫʽ
        Token t;
        this->SymbolTable.new_temp(1, t._value);
        ir.add_quad(Opcode::NE, SEM[SEM.size() - 2]._value, SEM[SEM.size() - 1]._value, t._value);
        SEM.pop_back();
        SEM.pop_back();
        SEM.push_back(t);
    }
    log( "����" );
}

void Parser::ConditionStatement_sp()
{
    if(PresentToken._type != TokenType::IF){
        err("�������ؼ���Ϊ if! ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("����ǰӦΪ ( ");
        
    }
    Next();
    Condition_sp();
    if (PresentToken._type != TokenType::RightBracket) {
        err("������ӦΪ ) ");
        
    }
    ir.add_quad(Opcode::IF, SEM.back()._value);
    SEM.pop_back();
    
    Next();
    // ��������
    INSCP();
    Statement_sp();
    // ��������
    OTSCP();
    if (PresentToken._type == TokenType::ELSE) {
        
        Next();
        ir.add_quad(Opcode::EL);
        // ��������
        INSCP();
        Statement_sp();
        // ��������
        OTSCP();
    }
    ir.add_quad(Opcode::IE);
    log( "�������" );
}
void Parser::LoopStatement_sp()
{
    if (PresentToken._type == TokenType::WHILE) {
        ir.add_quad(Opcode::WH);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("����ǰӦΪ ( ");
            
        }
        Next();
        Condition_sp();
        if (PresentToken._type != TokenType::RightBracket) {
            err("������ӦΪ ) ");
            
        }
        
        Next();
        ir.add_quad(Opcode::DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
        // ��������
        INSCP();
        Statement_sp();
        // ��������
        OTSCP();
        ir.add_quad(Opcode::WE);
    }
    else if (PresentToken._type == TokenType::DO) {
        ir.add_quad(Opcode::dw_LB);
        Next();
        // ��������
        INSCP();
        Statement_sp();
        // ��������
        OTSCP();
        if (PresentToken._type != TokenType::WHILE) {
            err("do-while���ȱ��while ");
            
        }
        ir.add_quad(Opcode::dw_WH);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("����ǰӦΪ ( ");
            
        }
        Next();
        Condition_sp();
        if (PresentToken._type != TokenType::RightBracket) {
            err("������ӦΪ ) ");
            
        }
        Next();
        if (PresentToken._type != TokenType::SemiColon) {
            err("do-while����ȱ�� ; �ָ��� ");
            
        }
        Next();
        ir.add_quad(Opcode::dw_DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
    }
    else if (PresentToken._type == TokenType::FOR) {
        ir.add_quad(Opcode::fr_FR);
        Next();
        if (PresentToken._type != TokenType::LeftBracket) {
            err("for����Ӧ�� ( ");
            
        }
        Next();
        AssignmentStatement_sp();
        if (PresentToken._type != TokenType::SemiColon) {
            err("��ֵ����ȱ�� ; �ָ��� ");
            
        }
        ir.add_quad(Opcode::fr_LB);
        Next();
        Condition_sp();
        ir.add_quad(Opcode::fr_DO, SEM[SEM.size() - 1]._value);
        SEM.pop_back();
        if (PresentToken._type != TokenType::SemiColon) {
            err("������ȱ�� ; �ָ��� ");
            
        }
        Next();
        // �Ѹ���������Ԫʽ�ŵ�ѭ�������Ԫʽ���棬����
        auto p1 = ir.get_cur_quad_position();
        p1++;
        AssignmentStatement_sp();
        auto p2 = ir.get_cur_quad_position();
        if (PresentToken._type != TokenType::RightBracket) {
            err("ȱ�� ) ");
            
        }
        Next();

        // ��������
        INSCP();
        Statement_sp();
        // ��������
        OTSCP();
        ir.move_backward(p1, p2);
        ir.add_quad(Opcode::fr_FE);
    }
    else {
        err("��ѭ�����ؼ��ִ��� ");
        
    }
    log( "ѭ�����" );
}


void Parser::Scanf_sp() {
    if (PresentToken._type != TokenType::SCANF) {
        err("�����scanf�ؼ��ִ��� ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("scanf��ӦΪ ( ");
        
    }
    Next();
    if (PresentToken._type != TokenType::Identifier) {
        err("��ʶ������ ");
        
    }
    UnStated_ERROR();
    // ��һ�±����øĺ����
    // �൱�ڸ�ֵ
    this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), PresentToken._value);
    ir.add_quad(Opcode::SCANF, PresentToken._value);
    Next();
    if (PresentToken._type != TokenType::RightBracket) {
        err("Ӧ�� )���� ");
        
    }
    Next();
    log( "�����" );
    
}

void Parser::Printf_sp() {
    if (PresentToken._type != TokenType::PRINTF) {
        err("д���printf�ؼ��ִ��� ");
        
    }
    Next();
    if (PresentToken._type != TokenType::LeftBracket) {
        err("printf��ӦΪ ( ");
        
    }
    Next();
    Expression_sp();
    ir.add_quad(Opcode::PRINTF, SEM[SEM.size() - 1]._value);
    if (PresentToken._type != TokenType::RightBracket) {
        err("Ӧ�� )���� ");
        
    }
    Next();
    log( "д���" );
   
}




void Parser::UnStated_ERROR()
{
    string temp;
    if (this->SymbolTable.serach_symbol(cvt_name(PresentToken._value, scope_level, scope_count), temp) == false)
    {
        err("��ʶ��" + PresentToken._value + "δ������");
    }
}




void Parser::RepeatStatement_ERROR()
{
    string temp; 
    string name = cvt_name(PresentToken._value, scope_level, scope_count);
    SymbolTable.serach_symbol(name, temp);
    if (temp == name)
    {
        err("��ʶ��" + PresentToken._value + "�ظ�������");
    }
}


