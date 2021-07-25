

#include <iostream>
#include "Lexer.h"
#include "IR.h"
#include "Parser.h"
#include "targetcode.h"
#include "optimize.h"   // 优化

void usage() {
    printf("使用:\n");
    printf("./simplc.exe filepath [-g] [-O]\n");
}
int main(int argc, char** argv)
{

    bool debug = false; // false时只输出 .asm 文件， true时输出各阶段的调试信息
    bool optimize = false; // false时不优化， true时优化

    if (argc > 3) {
        cout << "参数过多\n";
        usage();
        exit(1);
    }
    if (argv[1]) {
       //cout << "argv[1] : " << argv[1] << '\n';
    }
    else {
        cout << "请输入文件\n";
        exit(1);
    }
    if (argv[2]) {
        //cout << argv[2] << '\n';
        if (strcmp(argv[2], "-g") == 0) {
            cout << "调试模式\n";
            debug = true;
        }
        else if (strcmp(argv[2], "-O") == 0) {
            cout << "优化模式\n";
            optimize = true;
        }
        else if (strcmp(argv[2], "-Og") == 0 || strcmp(argv[2], "-gO") == 0) {
            cout << "优化 + 调试\n";
            debug = optimize = true;
        }
        else {
            cout << "参数错误\n";
            usage();
            exit(1);
        }
    }

    //char file_name[1000];
    //cout << "请输入源代码文件地址（绝对路径）：" << endl;
    //cin.get(file_name, 1000);
    //string source_file(file_name);

    string source_file(argv[1]);
    auto p = std::min(source_file.find_last_of('\\'), source_file.find_last_of('/'));
    string pref = source_file.substr(0, p + 1);
    if (p == string::npos) {
        p = 0;
    }else {
        p++;
    }
    string name = source_file.substr(p);
    auto p2 = name.find_last_of('.');
    name = name.substr(0, p2);

    Parser parser(source_file);
    // 无优化

    if (optimize == false) {
        
        parser.debug = debug;
        parser.set_output_file(pref + "lex_parse_res.txt");
        parser.Grammer_Lexer.init();
        parser.Program_sp();

        parser.ir.divide_blocks();
        parser.ir.cal_active_info_all();

        if (debug) {
            parser.ir.set_output_file(pref + "raw_quads.txt");
            parser.ir.debug();
            parser.SymbolTable.print_all_scope(pref + "scope.txt");
            parser.SymbolTable.print_all_table(pref + "symbol_table.txt");
        }

    }
    else {
        //优化版
        parser.debug = debug;
        parser.set_output_file(pref + "lex_parse_res.txt");
        parser.Grammer_Lexer.init();
        parser.Program_sp();

        parser.ir.divide_blocks();

        if (debug) {
            parser.ir.cal_active_info_all();
            parser.ir.set_output_file(pref + "raw_quads.txt");
            parser.ir.debug();
        }

        DAG dag;
        BasicBlock b;
        parser.ir.cur_block = parser.ir.all_blocks.begin();
        while (parser.ir.next_block(b)) {
            dag.GenerateDAG(parser.ir.all_quads, b);
        }
        parser.ir.all_quads.clear();
        parser.ir.all_quads.resize(dag.last_quads.size());
        parser.ir.all_quads = dag.last_quads;
        parser.ir.all_blocks.clear();


        parser.ir.divide_blocks();
        parser.ir.cal_active_info_all();

        if (debug) {
            parser.ir.set_output_file(pref + "optimaized_quads.txt");
            parser.ir.debug();
            parser.SymbolTable.print_all_scope(pref + "scope.txt");
            parser.SymbolTable.print_all_table(pref + "symbol_table.txt");
        }

    }
    

    targetcode tc;
    tc.input(parser.ir.get_all_quads());
    tc.deal();
    tc.out(pref + name + ".asm");
    


    return 0;
}

