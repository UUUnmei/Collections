using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using System.Threading;
using System.Security.Permissions;
using System.Windows.Threading;

namespace HelloWPF
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();

        }

        public void drawTriangle(int col)
        {
            //在第二行的指定列画三角形
            //第二列的小格子长47.59 高19.36
            //用polygon

            PointCollection pc = new PointCollection();
            pc.Add(new Point(24, 0.5));
            pc.Add(new Point(1, 18));
            pc.Add(new Point(47, 18));
            poly.Points = pc;
            poly.Fill = Brushes.Gold;
            
            Grid.SetColumn(poly, col);

        }

        private void OpenFile(object sender, RoutedEventArgs e)
        {
            //载入已有图灵机按钮 
            OpenFileDialog openFileDialog = new OpenFileDialog();
            //openFileDialog.InitialDirectory = @"D:\Visual Studio\";    //指定初始文件夹
            openFileDialog.InitialDirectory = Environment.CurrentDirectory; //从当前程序所在文件夹开始
            openFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";     //文件类型过滤器
            if(openFileDialog.ShowDialog() == true)
            {
                which.Text = "当前模式： " + System.IO.Path.GetFileNameWithoutExtension(openFileDialog.FileName);


                var fileStream = openFileDialog.OpenFile();
                System.IO.StreamReader reader = new System.IO.StreamReader(fileStream);
                //获取文件行数
                int len = 0;
                while (reader.ReadLine() != null)
                {
                    len++;
                }

                reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);  //返回开头
                initLeft(reader);       //初始化左侧参数
                initList(reader, len);  //初始化右侧列表中参数

            }
        }

        public void initLeft(System.IO.StreamReader reader)
        {
            string[] t = reader.ReadLine().Split(' ');
            K.Text = string.Join(", ", t);
            t = reader.ReadLine().Split(' ');
            Sigma.Text = string.Join(", ", t);
            t = reader.ReadLine().Split(' ');
            Gamma.Text = string.Join(", ", t);
            string a = reader.ReadLine();
            q0.Text = a;
            cur_state = a;

            t = reader.ReadLine().Split(' ');
            F.Text = string.Join(", ", t);
        }

        public void reset()
        {
            //每次重新读输入串，对下面重绘
            for (int i = 0; i < 16; i++)
            {
                box.tb[i].Text = "B";
                box.rect[i].Fill = Brushes.LightGray;
            }
            //重置参数
            curIdx = 0;
            cur_state = q0.Text;
        }
        string data;
        private void ReadInput(object sender, RoutedEventArgs e)
        {
            //按下 读取输入串 按钮后进行读取
            data = Input.Text.ToString();
            //MessageBox.Show(data);  //展示一下，作为测试
            reset();
            for(int i = 0; i < data.Length; i++)
            {
                box.tb[i + 1].Text = data[i].ToString();
            }

        }

        private List<string> FillList(Rule[] r)
        {
            // 右侧规则列表
            List<string> Delta = new List<string>();
            for(int i = 0; i < r.Length; i++)
            {
                Rule d = r[i];
                Delta.Add(d.Normalize(d));
            }
            return Delta;
        }

        Rule[] r;
        public void initList(System.IO.StreamReader reader, int len)
        {
            string line;
            r = new Rule[len-5];
            int i = 0;
            while ((line = reader.ReadLine()) != null)
            {
                r[i] = new Rule(line.Split(' '));
                i++;
            }
            

            Rules.ItemsSource = FillList(r);

            //为右侧的listbox，可以用鼠标滚轮控制滚动条
            Rules.PreviewMouseWheel += (sender, e) =>
            {
                var eventArg = new MouseWheelEventArgs(e.MouseDevice, e.Timestamp, e.Delta);
                eventArg.RoutedEvent = MouseWheelEvent;
                Rules.RaiseEvent(eventArg);
            };

            /*
            Rules.SelectedIndex = 10;      // 选择listbox中的第几项
            Rules.ScrollIntoView(Rules.SelectedItem); //自动滚动到该位置*/
        }

        
        
        

        public class Rule   //规则的类
        {
            public enum Move { L = -1, S, R };

            public string curState;
            public string curChar;
            public string nxtState;
            public string nxtChar;
            public Move move;

            public Rule() { }

            public Rule(string[] s)
            {
                curState = s[0];
                curChar = s[1];
                nxtState = s[2];
                nxtChar = s[3];
                if (s[4] == "L")
                    move = Move.L;
                else if (s[4] == "R")
                    move = Move.R;
                else
                    move = Move.S;
            }

            public string Normalize(Rule r)
            {
                string mv;
                if (r.move == Move.L)
                    mv = "L";
                else if (r.move == Move.R)
                    mv = "R";
                else
                    mv = "S";

                return string.Format("δ({0},{1}) = ({2}, {3}, {4})", r.curState, r.curChar, r.nxtState, r.nxtChar, mv);

            }


        }
        public class Box
        {
            public Rectangle[] rect;
            public TextBlock[] tb;
        }
        Box box = new Box();
        //画下面的展示效果
        private void Window_ContentRendered(object sender, EventArgs e)
        {

            //结合xaml的写法。。。

            TextBlock[] tbs = new TextBlock[] { tb1, tb2, tb3, tb4, tb5, tb6, tb7, tb8, tb9, tb10, tb11, tb12, tb13, tb14, tb15, tb16 };
            Rectangle[] rects = new Rectangle[] { rect1, rect2, rect3, rect4, rect5, rect6, rect7, rect8, rect9, rect10, rect11, rect12, rect13, rect14, rect15, rect16 };

            box.rect = rects;
            box.tb = tbs;

            reset();

            //可见xaml中注释掉的部分
            //代码的写法

            ////格子基本样式
            //grid.Height = 98;
            //grid.Width = 768;
            //grid.Margin = new Thickness(10, 10, 0, 0);

            ////设置行
            //RowDefinition row1 = new RowDefinition()
            //{
            //    Height = new GridLength(4, GridUnitType.Star)
            //};
            //RowDefinition row2 = new RowDefinition()
            //{
            //    Height = new GridLength(1, GridUnitType.Star)
            //};
            //grid.RowDefinitions.Add(row1);
            //grid.RowDefinitions.Add(row2);

            ////设置列
            //ColumnDefinition[] cols = new ColumnDefinition[16];
            //for (int i = 0; i < 16; i++)
            //{
            //    cols[i] = new ColumnDefinition();
            //    cols[i].Width = new GridLength(1, GridUnitType.Star);
            //    grid.ColumnDefinitions.Add(cols[i]);
            //}

            ////每个格子的内容
            //Box[] elems = new Box[16];
            //for (int i = 0; i < 16; i++)
            //{
            //    elems[i] = new Box();
            //    elems[i].rect = new Rectangle()
            //    {
            //        Fill = Brushes.Red
            //    };
            //    elems[i].tb = new TextBlock()
            //    {
            //        Text = i.ToString()
            //    };
            //    Grid.SetColumn(elems[i].rect, i);
            //    Grid.SetColumn(elems[i].tb, i);
            //    Grid.SetRow(elems[i].rect, 0);
            //    Grid.SetRow(elems[i].tb, 0);

            //    grid.Children.Add(elems[i].rect);
            //    grid.Children.Add(elems[i].tb);
            //}
        }
        int curIdx = 0;
        string cur_state;
        int ex_i = 0;
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            //按下 运行 按钮随机改变下面的块  ---测试

            //Random rand = new Random();
            //int i = rand.Next(16);
            //box.rect[i].Fill = new SolidColorBrush(Color.FromRgb((byte)rand.Next(0, 255), (byte)rand.Next(0, 255), (byte)rand.Next(0, 255)));
            //box.tb[i].Text = i.ToString();
            drawTriangle(curIdx + 1);
            string curC = box.tb[curIdx+1].Text;    //当前读到的字符
            int i = 0;
            for (; i < r.Length; i++)
            {
                
                //找当前状态在第几个规则
                if(cur_state == r[i].curState)  //状态相同
                {
                    if(curC == r[i].curChar)    //读入字符相同
                    {
                        //则按照规则执行
                        //更新状态
                        curC = r[i].nxtChar;
                        cur_state = r[i].nxtState;


                        //更新画面
                        //右侧列表
                        Rules.SelectedIndex = i;      // 选择listbox中的第几项
                        Rules.ScrollIntoView(Rules.SelectedItem); //自动滚动到该位置*/
                        //下方
                        box.tb[curIdx + 1].Text = curC;
                        box.rect[curIdx + 1].Fill = Brushes.Red;

                        curIdx += (int)r[i].move;
                        break;
                    }
              
                }

            }
            ex_i = i;
            if (cur_state == F.Text)
                MessageBox.Show("接受" + data, "结果");
            else if (i == r.Length)
                MessageBox.Show("不接受" + data, "结果");
            


        }


        //为了避免Thread.Sleep造成界面假死   --start//
        public static class DispatcherHelper
        {
            [SecurityPermissionAttribute(SecurityAction.Demand, Flags = SecurityPermissionFlag.UnmanagedCode)]
            public static void DoEvents()
            {
                DispatcherFrame frame = new DispatcherFrame();
                Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background, new DispatcherOperationCallback(ExitFrames), frame);
                try { Dispatcher.PushFrame(frame); }
                catch (InvalidOperationException) { }
            }
            private static object ExitFrames(object frame)
            {
                ((DispatcherFrame)frame).Continue = false;
                return null;
            }
        }
        public static void Delay(int mm)
        {
            DateTime current = DateTime.Now;
            while (current.AddMilliseconds(mm) > DateTime.Now)
            {
                DispatcherHelper.DoEvents();
            }
            return;
        }
        //为了避免Thread.Sleep造成界面假死   --end//
        //但是每次间隔的时间并不完全由Delay的参数控制，时快时慢。。。。


        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            //模拟按下 运行 按钮直到结束
            
            while (cur_state != F.Text && ex_i!=r.Length)
            {
                oneStep.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
                Delay(100);
                
            }

        }
    }
}
