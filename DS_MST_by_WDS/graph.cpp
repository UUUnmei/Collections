#include "graph.h"
#include "drawwidget.h"
#include "QPair"
#include "QDebug"
void drawWidget::setSpeed(int v)
{
//    qDebug() << v;
    speed = (100 - v) * 10;
}

QListWidgetItem* setItem(QString str)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(str);
    item->setFont(getFont());
    item->setFlags(Qt::ItemIsEnabled);
    return item;
}

void drawWidget::setListWidget(QString str, QString queue = "")
{
    list->clear();
    if(isShowBoard) list->show();
    if (str == "dfs"){
        list->setGeometry(950, 560, 500, 400);
        item[0] = setItem("function dfs(u):");
        item[1] = setItem("    for each neighbour v of u:");
        item[2] = setItem("        if v has not been visted:");
        item[3] = setItem("            dfs(v)");
        for (int i = 0; i < 4; i++)
            list->addItem(item[i]);
    }
    else if (str == "bfs"){
        list->setGeometry(850, 560, 600, 400);
        item[0] = setItem("queue q = { " + queue + " }");
        item[1] = setItem("function bfs():");
        item[2] = setItem("    while q.empty() == false:");
        item[3] = setItem("        u = q.front(), q.pop()");
        item[4] = setItem("        for each nerighbour v of u:");
        item[5] = setItem("            if v has not been visted:");
        item[6] = setItem("                q.push(v)");
        for (int i = 0; i < 7; i++)
            list->addItem(item[i]);
    }
    else if (str == "prime"){
        list->setGeometry(750, 560, 600, 400);
        item[0] = setItem("set = { " + queue + " }");
        item[1] = setItem("funciton prime(u)");
        item[2] = setItem("     if(value<min&&vis[j]==false&&i!=j)");
        item[3] = setItem("         find min edge of piont");
        item[4] = setItem("     take it in the tree");
        item[5] = setItem("end");
        for (int i = 0; i < 6; i++)
            list->addItem(item[i]);
    }
    else if(str == "Kruskal"){
            list->setGeometry(850, 560, 600, 400);
            item[0] = setItem("sort edges into nondecreasing order by weight");
            item[1] = setItem("for each vertex v:");
            item[2] = setItem("    make v into set");
            item[3] = setItem("function kruskal():");
            item[4] = setItem("    for each sorted edge(u, v):");
            item[5] = setItem("        if set of u != set of v:");
            item[6] = setItem("            union(u, v)");

            for(int i = 0; i < 7; i++)
                list->addItem(item[i]);
        }
}

void drawWidget::algorithm(QString str, int begID)
{
    if (begID < 0 || begID > graph.size()) return;
    isRun = 1;
    repaint();
    if (begID >= graph.size()) {
        isRun = 0; list->hide();
        return;
    }
    memset(vis, 0, sizeof vis);
    if (str == "dfs") setListWidget(str), dfs(begID);
    if (str == "bfs") setListWidget(str, QString::number(begID)), bfs(begID);
    if (str == "prime") setListWidget(str, QString::number(begID)), prime(begID);
    if(str=="Kruskal"){
            setListWidget(str);
            kruskal();
        }
    isRun = 0;
    list->hide();
}

void drawWidget::dfs(int u)
{
    Circle tot = graph.circle(u);
    vis[u] = 1;

//    sleep(300);
    item[0]->setForeground(Qt::red);
    item[3]->setForeground(Qt::black);
    drawCircle(tot, QColor(154, 255, 154));
    sleep(speed);


    for (int i = 0; i < graph.size(); i++){
        if (i == u || !graph.edge(u, i)||vis[i]) continue;

        item[0]-> setForeground(Qt::black);
        item[1]-> setForeground(Qt::red);

        drawLineAnimation(u, i, QColor(154, 255, 154));
        Circle v = graph.circle(i);
        drawCircle(v, QColor(154, 255, 154));
        drawCircle(tot, tot.color());

        sleep(speed);

        item[1]-> setForeground(Qt::black);
        item[2]-> setForeground(Qt::red);

        cleanTemp();

        if (!vis[i]){
            sleep(speed);
            item[2]-> setForeground(Qt::black);
            item[3]-> setForeground(Qt::red);
            sleep(speed);
            dfs(i);
        }
        else {
            sleep(speed);
            item[2]-> setForeground(Qt::black);
            drawLine(u, i, Qt::gray, defaultLineSize + 1);
            drawCircle(v, v.color());
        }

        drawCircle(tot, QColor(154, 255, 154));
        item[1]-> setForeground(Qt::black);

        sleep(speed / 5);
    }

    sleep(speed);
    drawCircle(tot, tot.color());
    sleep(speed);
}

QString getQueue(QVector<int> v)
{
    QString s = "";
    if (v.empty()) return s;
    int last = v.back(); v.pop_back();
    for (auto &x : v){
        s += QString::number(x) + ", ";
    }
    s += QString::number(last);
    return s;
}

void drawWidget::bfs(int beg)
{
    std::queue<int> q; q.push(beg); vis[beg] = 1;
    QVector<int> vec; vec.append(beg);

    item[1]-> setForeground(Qt::red);
    sleep(speed);
    item[1]-> setForeground(Qt::black);

    while (!q.empty()){
        item[2]-> setForeground(Qt::red);
        sleep(speed);

        int u = q.front(); q.pop(); vec.pop_front();
        setListWidget("bfs", getQueue(vec));
        Circle tot = graph.circle(u);

        item[2]-> setForeground(Qt::black);
        item[3]-> setForeground(Qt::red);
        drawCircle(tot, QColor(154, 255, 154));
        sleep(speed);

        for (int i = 0; i < graph.size(); i++){
            if (i == u || !graph.edge(u, i)||vis[i]) continue;

            item[3]-> setForeground(Qt::black);
            item[4]-> setForeground(Qt::red);

            drawLineAnimation(u, i, QColor(154, 255, 154));
            Circle v = graph.circle(i);
            drawCircle(v, QColor(154, 255, 154));
            drawCircle(tot, tot.color());
            sleep(speed);

            cleanTemp();

            item[4]-> setForeground(Qt::black);
            item[5]-> setForeground(Qt::red);
            sleep(speed);

            if (!vis[i]){
                vec.append(i);
                setListWidget("bfs", getQueue(vec));
                item[5]-> setForeground(Qt::black);
                item[6]-> setForeground(Qt::red);
                sleep(speed);

                vis[i] = 1;
                q.push(i);
            }
            else {
                item[5]-> setForeground(Qt::black);
                sleep(speed);
                drawLine(u, i, Qt::gray, defaultLineSize + 1);
            }

            drawCircle(v, v.color());
            drawCircle(tot, QColor(154, 255, 154));
            item[6]-> setForeground(Qt::black);
            sleep(speed / 3);
        }

        sleep(speed);
        item[3]-> setForeground(Qt::black);
        drawCircle(tot, tot.color());
        sleep(speed);
    }
}

void drawWidget::count_bfs(int beg,int* p,int* flag){
    flag[beg]=1;
    (*p)++;
    for(int i=0;i<graph.size();i++){
        if(graph.edgeID(beg,i)!=-1&&beg!=i&&flag[i]==0){
            count_bfs(i,p,flag);
        }
    }
}

int drawWidget::number_tree(int beg){
    int flag[100],num=0;
    int *p;
    p=&num;
    memset(flag,0,sizeof(flag));
    count_bfs(beg,p,flag);
    return num;
}

void drawWidget::prime(int beg){
    vis[beg]=true;
    QVector<int> vec;
    vec.push_back(beg);
    drawCircle(graph.circle(beg),QColor(154, 255, 154));
    int numberofthis=number_tree(beg);
    int min_flag,flag=0,i_path=0;
    while(!flag){
        int count=0;
        for(int i=0;i<graph.size();i++){
            if(vis[i])
                count++;
            if(count==numberofthis)
                flag=1;
        }
        for(int i=0;i<graph.size();i++){        //找到需要添加的点
            if(vis[i]==true){
                int min=10000,j;
                for(j=0;j<graph.size();j++){
                    if(graph.edgeID(i,j)!=-1){
                        int value=graph.edge(i,j);
                        if(value<min&&vis[j]==false&&i!=j){
                            min=value;
                            min_flag=j;
                            i_path=i;
                        }
                    }
                }
            }

            }
        if(flag!=1){
            item[0]->setForeground(Qt::red);
            sleep(speed);
            item[0]->setForeground(Qt::black);
            item[1]->setForeground(Qt::red);
            vec.push_back(min_flag);
            sleep(speed);
            item[1]->setForeground(Qt::black);
            sleep(speed);
            item[2]->setForeground(Qt::red);
            sleep(speed);
            item[2]->setForeground(Qt::black);
            item[3]->setForeground(Qt::red);
            sleep(speed);
            drawCircle(graph.circle(min_flag),QColor(154, 255, 154));
            sleep(speed);
            setListWidget("prime", getQueue(vec));
            item[0]->setForeground(Qt::red);
            item[3]->setForeground(Qt::black);
            item[0]->setForeground(Qt::black);
            sleep(speed);
            vis[min_flag]=true;
            item[4]->setForeground(Qt::red);
            drawLineAnimation(i_path, min_flag, QColor(154, 255, 154));
            sleep(speed);
            item[4]->setForeground(Qt::black);
        }
        }
    item[5]->setForeground(Qt::red);
    sleep(speed);
}

/* 并查集 */
int fa[105];
int getFa(int x){
    return fa[x]==x?x:(fa[x]=getFa(fa[x]));
}

/* 图 */
struct EDGE{
    int u,v,w;
}edges[200];
int cnt=0;
void initEdges(Graph g){
    for(int i=0;i<g.size();i++)
        for(int j=0;j<g.size();j++)
            if(g.edgeID(i,j)!=-1){
                edges[cnt].u = i;
                edges[cnt].v = j;
                edges[cnt].w = g.edge(i,j);
                cnt++;
            }
}

/* 堆排序 */
void PercDown( struct EDGE A[], int p, int N )
{
    int Parent, Child;
    struct EDGE X;

    X = A[p]; /* 取出根结点存放的值 */
    for( Parent=p; (Parent*2+1)<N; Parent=Child ) {
        Child = Parent * 2 + 1;
        if( (Child!=N-1) && (A[Child].w<A[Child+1].w) )
            Child++;  /* Child指向左右子结点的较大者 */
        if( X.w >= A[Child].w ) break; /* 找到了合适位置 */
        else  /* 下滤X */
            A[Parent] = A[Child];
    }
    A[Parent] = X;
}

void HeapSort( struct EDGE A[], int N )
{
     int i;
    /* 建立大顶堆，但全部排序后是升序 */
     for ( i=N/2-1; i>=0; i-- )            //下标从0开始
         PercDown( A, i, N );
    /* 删除堆顶 */
     for ( i=N-1; i>0; i-- ) {
         std::swap(A[0], A[i]);
         PercDown( A, 0, i );
     }
}

void drawWidget::kruskal()
{
    initEdges(graph);
    item[0]->setForeground(Qt::red);
    HeapSort(edges, cnt);
    sleep(speed);
    item[0]->setForeground(Qt::black);


    for(int i=1;i<=graph.size();i++){
        item[1]->setForeground(Qt::red);
        sleep(speed);
        item[1]->setForeground(Qt::black);
        item[2]->setForeground(Qt::red);
        fa[i] = i;
        drawCircle(graph.circle(i-1), Qt::white);
        sleep(speed);
        item[2]->setForeground(Qt::black);
    }

    item[3]->setForeground(Qt::red);
    sleep(speed);
    item[3]->setForeground(Qt::black);

    int tot = 0;
    int num = 0;
    for(int i=0;i<cnt;i++) {
        item[6]->setForeground(Qt::black);
        item[4]->setForeground(Qt::red);
        sleep(speed);
        item[4]->setForeground(Qt::black);

        item[5]->setForeground(Qt::red);
        int rt1 = getFa(edges[i].u + 1);
        int rt2 = getFa(edges[i].v + 1);
        if(rt1==rt2){
            sleep(speed);
            item[5]->setForeground(Qt::black);
            continue;
        }
        sleep(speed);
        item[5]->setForeground(Qt::black);

        item[6]->setForeground(Qt::red);
        drawLineAnimation(edges[i].u, edges[i].v, Qt::red);
        tot += edges[i].w;
        fa[rt2] = rt1;
        //让 u 与 v 的颜色相同表示是同一集合
        drawCircle(graph.circle(edges[i].u), Qt::yellow);
        drawCircle(graph.circle(edges[i].v), Qt::yellow);

        sleep(speed / 5);
        if((++num)==graph.size()-1)
            break;
    }
    if(num == graph.size()-1)
        qDebug() << tot <<endl;
    else
        qDebug() << -1 <<endl;
}
/* kruskal */
