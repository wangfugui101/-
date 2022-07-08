#include "music_ui.h"
#include "novel_ui.h"
#include "ui_music_ui.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QtDebug>
#include <stdlib.h>
Music_UI::Music_UI(QString name,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Music_UI)
{
    ui->setupUi(this);
    ui->music_name->setText(name);
    ui->progressBar->setValue(0);

    connect(&mtimer,&QTimer::timeout,this,&Music_UI::timerun);
    mtimer.start(1000);

    //设置好进程的重定向---交给其他接口
    mprocess.setProcessChannelMode(QProcess::MergedChannels);

    //关联当进程有数据可以读取时，交给music_readdata输出出来
    connect(&mprocess,&QProcess::readyReadStandardOutput,this,&Music_UI::music_readdata);

    //关联定时器的槽函数
    connect(&mtimer,&QTimer::timeout,this,&Music_UI::timer_run);
    //关联网络请求
    connect(&manager,&QNetworkAccessManager::finished,this,&Music_UI::readdata);
}

Music_UI::~Music_UI()
{
    delete ui;
}


void Music_UI::timerun()
{
    mdate = QDate::currentDate();//获取当前日期
    QString date_str = mdate.toString("yyyy-MM-dd");
    mtime = QTime::currentTime();//获取当前时间
    QString time_str = mtime.toString("HH:mm:ss");
    QString time = date_str+"  "+time_str;
    ui->music_time->setText(time);
}

void Music_UI::music_readdata()
{
    QString str =  mprocess.readAll();

    //qDebug()<< str;
    // indexOf()判断是否包含某个字符串，如果有，返回该字符串在str中的位置下标，没有返回-1
    if( str.indexOf("ANS_LENGTH=") != -1)
    {
        //删除多余的字符串
        str.remove("ANS_LENGTH=");
        str.remove("\r\n");

        //先把字符串转成浮点数，再把浮点数转成整型数值
        int total = static_cast<int>(str.toFloat());

        //把滑动条的最大值设置完成
        ui->progressBar->setMaximum(total);

        //转换时间
        total_time.setHMS(0,total/60,total%60);

        //把时间写入到标签中
        ui->music_total_time->setText(total_time.toString("mm:ss"));
    }
    else  if( str.indexOf("ANS_TIME_POSITION=") != -1)
    {
        //删除多余的字符串
        str.remove("ANS_TIME_POSITION=");
        str.remove("\r\n");

        //先把字符串转成浮点数，再把浮点数转成整型数值
        int now = static_cast<int>(str.toFloat());

        //设置滑动条的当前数值
        ui->progressBar->setValue(now);


        //转换时间
        now_time.setHMS(0,now/60,now%60);

        //把时间写入到标签中
        ui->music_now_time->setText(now_time.toString("mm:ss"));
    }
}

void Music_UI::timer_run()
{
    mprocess.write("get_time_pos\r\n");
}

void Music_UI::on_music_setting_clicked()
{
    QString filepath =QFileDialog::getOpenFileName(this);
    QString tmp="Music_UI{border-image: url(:/";
    QFileInfo fileinfo(filepath);
    QString filename = fileinfo.fileName();
    tmp = tmp+filename+");}QPushButton{background-color: rgb(0, 255, 0);}";
    this->setStyleSheet(tmp);
}

void Music_UI::on_music_minimize_clicked()
{
    this->showMinimized();
}

void Music_UI::on_music_exit_clicked()
{
    //判断当前是否有音频正在播放---也就是进程正在运行
    if( mprocess.state() == QProcess::Running )
    {
        mprocess.kill();//杀死当前进程---只是发送了finished的信号，还需要时间等待进程释放资源完毕
        mprocess.waitForFinished();
    }

    this->parentWidget()->show();
    delete this;
}

void Music_UI::on_music_prev_clicked()
{
    //获取当前行数和总行数----如果有3行，顺序应该是0,1,2
    int total = ui->music_library->count();
    int now   = ui->music_library->currentRow();

    now--;
    if(now==-1)
        now = total-1;

    //让窗口选中第now行
    ui->music_library->setCurrentRow(now);

    on_music_play_clicked();
}

void Music_UI::on_music_add_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this);
    if(filepath=="")
        return ;
    //判断mylist是否有该路劲
    QList<QString>::Iterator it ;
    for (it = mylist.begin();it != mylist.end(); it++){
        if(*it == filepath)
        {
            qDebug()<<"重复！";
            return;
        }
    }
    //将绝对路劲存进mylist
    mylist.append(filepath);
    //获取MP3的文件名
    QFileInfo fileinfo(filepath);
    QString filename = fileinfo.fileName();
    ui->music_library->addItem(filename);
//    qDebug()<<ui->music_library->count();
}

void Music_UI::on_music_play_clicked()
{
    //获取当前选中的是哪个窗口---没有选中的就退出
    QListWidgetItem *item = ui->music_library->currentItem();
    if(item==nullptr)
        return ;

    //拿到该窗口的文件名
    QString filepath = item->text();
    QString songname = filepath.remove(".mp3");
    ui->music_song_name->setText(songname);
    //判断当前是否有音频正在播放---也就是进程正在运行
    if( mprocess.state() == QProcess::Running )
    {
        mprocess.kill();//杀死当前进程---只是发送了finished的信号，还需要时间等待进程释放资源完毕
        mprocess.waitForFinished();
    }

    //播放音频文件
    int tmp=ui->music_library->currentRow();
    QStringList list;
    list  << "-slave" << "-quiet" << mylist.at(tmp);
    mprocess.start("D:/yueqian/mplayer/mplayer.exe",list);

    mtimer.start(1000);//1秒触发一次

    //写入指令---获取当前音频长度
    mprocess.write("get_time_length\r\n");
}

void Music_UI::on_music_next_clicked()
{
    //获取当前行数和总行数----如果有3行，顺序应该是0,1,2
    int total = ui->music_library->count();
    int now   = ui->music_library->currentRow();

    now++;
    if(now == total)
        now = 0;

    //让窗口选中第now行
    ui->music_library->setCurrentRow(now);

    on_music_play_clicked();
}

void Music_UI::on_music_del_clicked()
{
    //获取当前选中的是哪个窗口---没有选中的就退出
    QListWidgetItem *item = ui->music_library->currentItem();
    if(item==nullptr)
        return ;

    //拿到该窗口的文件名
    QString filepath = item->text();
    //qDebug()<<filepath;
    //判断当前是否有音频正在播放---也就是进程正在运行
    if( mprocess.state() == QProcess::Running )
    {
        mprocess.kill();//杀死当前进程---只是发送了finished的信号，还需要时间等待进程释放资源完毕
        mprocess.waitForFinished();
    }
    //删除listwiget的一条内容，同时删除mylist的一条内容
    int tmp=ui->music_library->currentRow();
    ui->music_library->takeItem(tmp);
    mylist.removeAt(tmp);
}

void Music_UI::on_music_empty_clicked()
{
    //获取总行数
    int num = ui->music_library->count();
    //判断当前是否有音频正在播放---也就是进程正在运行
    if( mprocess.state() == QProcess::Running )
    {
        mprocess.kill();//杀死当前进程---只是发送了finished的信号，还需要时间等待进程释放资源完毕
        mprocess.waitForFinished();
    }
    //遍历总行数删除每一行
    for (int i = 0; i <num ; i++)
    {
        QListWidgetItem *item = ui->music_library->takeItem(0);
        if(item)
            delete item;
        item = nullptr;
    }
    //清空mylist的所有内容
    mylist.clear();

}

void Music_UI::on_search_clicked()
{
    QString tmp = ui->music_search->text();
    int i =0,num = ui->music_library->count();
    for (i=0;i<num;i++) {
        QString songname =ui->music_library->item(i)->text();
        if(songname.contains(tmp))
        {
            ui->music_library->setCurrentRow(i);
            return;
        }
    }
    //qDebug()<<"i="<<i<<"num="<<num;
    if(i == num)
    {
        QMessageBox::warning(this,"警告信息","未找到！");
    }
}

void Music_UI::on_horizontalSlider_sliderMoved(int position)
{
    ui->music_value->setNum(position);
}

void Music_UI::on_horizontalSlider_sliderReleased()
{
    QString s = ui->music_value->text();
    //qDebug()<<s;
    int s1 = s.toInt();
    qDebug()<<s1;

    s = QString("%1").arg(s1);
    //QString s = QString("%1").arg(position-10);
    QString tmp = "volume "+s+" 1";
    QByteArray t = tmp.toUtf8();
    mprocess.write(t);
}

void Music_UI::on_novel_search_clicked()
{
    qDebug("下面正在根据信息开始爬取列表...");
    QString tmp = "http://www.cqwsjds.com/ar.php?keyWord="+ui->lineEdit->text();
    QUrl url(tmp);
    QNetworkRequest request(url);
    manager.get(request);
    //点击搜索按钮后，如果listwidget里面有数据，就将listwidget清空和存放搜索界面的链接的hreflist也清空。
    int num = ui->listWidget->count();
    //qDebug()<<num;
    if(num != 0)
    {
        for (int i =0;i<num;i++) {
            ui->listWidget->takeItem(0);
        }
        hreflist.clear();
    }
}

void Music_UI::readdata(QNetworkReply *reply)
{
//http://www.cqwsjds.com/ar.php?keyWord=
    QString s = reply->readAll();
    //qDebug()<<s;
    //ui->textEdit->setText(s);
    QRegExp rx;

    //rx.setPattern("\\举报后维护人员会在两分钟内校正章节内容,请耐心等待,并刷新页面。</div>(.*)<br />\\s");
    //定义查询的正则表达式
    rx.setPattern("\\<ul class=\"txt-list txt-list-row5\">(.*)</ul>\\s");
    //判断正则表达式是否书写正确
    //qDebug() << rx.isValid();
    //设置为非贪心匹配
    rx.setMinimal(true);
    int pos = rx.indexIn(s);
    //qDebug() <<"pos="<<pos;
    if (pos > -1) {
         value = rx.cap(1);
         value.replace("\r\n","　　");
         value.remove("            ");
         //qDebug()<<value;
     }
    int a;
    //循环将每一个书籍放进listwidget里面
    while (1) {
        a=getList();
        if(a==-1)
        {
            break;
        }
        getList();
    }
    value.remove("<li>");
    value.remove("</li>");
    ui->listWidget->addItem(value);
    ui->listWidget->takeItem(0);
    ui->listWidget->takeItem(0);
    remove();
    gethref();
    qDebug()<<"列表爬取成功！";
}
//将爬取的内容存进listwidget里面
int Music_UI::getList()
{
    QRegExp rx;
    //定义查询的正则表达式
    rx.setPattern("\\<li>(.*)</li>\\s");
    //判断正则表达式是否书写正确
    //qDebug() << rx.isValid();
    //设置为非贪心匹配
    rx.setMinimal(true);
    int pos = rx.indexIn(value);
    //qDebug() <<"pos="<<pos;
    //获取正则表达式所获取的内容并去除杂项
    if (pos != -1) {
        QString tmp = rx.cap(1);
        value=value.simplified();
        value.remove(tmp);
        value.remove("<li></li> ");
        ui->listWidget->addItem(tmp);
     }
    return pos;
}
//修改listwidget的文本删除了span
void Music_UI::remove()
{
    int num = ui->listWidget->count();
    for(int i =0 ;i<num;i++)
    {
        QString tmp = ui->listWidget->item(i)->text();
        tmp.remove("<span class=\"s1\">");
        tmp.remove("<span class=\"s2\">");
        tmp.remove("<span class=\"s3\">");
        tmp.remove("<span class=\"s4\">");
        tmp.remove("<span class=\"s5\">");
        tmp.remove("</span>");
        ui->listWidget->item(i)->setText(tmp);
    }
}
//修改listwidget的文本并且将对应的连接存储进书本连接hreflist
void Music_UI::gethref()
{
    int num = ui->listWidget->count();
    QRegExp rx;
    rx.setPattern("</a>  <a href=\"(.*).html\">");
    for(int i =0 ;i<num;i++)
    {
        QString tmp = ui->listWidget->item(i)->text();
        int pos = rx.indexIn(tmp);
        rx.setMinimal(true);
        //qDebug()<<pos;
        if (pos != -1) {
            QString s = rx.cap(1);
            tmp.remove(s);
            //合并所有的空格变成“ ”;
            tmp=tmp.simplified();
            tmp.replace("</a> <a href=\".html\">"," ");
            tmp.remove("</a>");
            //qDebug()<<tmp;

            QRegExp rx1;
            rx1.setPattern("<a (.*)>");
            rx1.indexIn(tmp);
            QString s1 = rx1.cap(1);
            //qDebug()<<"s1="<<s1;
            tmp.remove(s1);
            tmp.remove("<a >");
            //qDebug()<<"tmp"<<tmp;
            s1.remove("href=\"");
            s1.remove("\"");
            //qDebug()<<s1;
            s = "http://www.cqwsjds.com"+s1;
            //qDebug()<<s;
            hreflist.append(s);
         }
        ui->listWidget->item(i)->setText(tmp);
    }
}

void Music_UI::on_pushButton_clicked()
{
    int row = ui->listWidget->currentRow();
    //qDebug()<<row;
    QString tmp = hreflist.at(row);
    //qDebug()<<tmp;
    //创建新界面
    Novel_UI *p = new Novel_UI(tmp,this);
    p->show();

    //隐藏当前界面
    this->hide();
}
