from django.shortcuts import render
from django.shortcuts import redirect
from django import forms
import json
import os
from django.db import connections
from django.http import HttpResponse
from django.contrib import messages
from django.core.files.storage import default_storage


from operator import itemgetter
from itertools import groupby
import xlrd
import datetime
import base64

import requests
import lxml
from bs4 import BeautifulSoup

key = "123"

def dictfetchall(cursor):
	desc = cursor.description
	return [
	dict(zip([col[0] for col in desc], row))
		for row in cursor.fetchall()
		]

class UserForm(forms.Form):
    username = forms.CharField(label="用户名", max_length=128, widget=forms.TextInput(attrs={'class': 'form-control'}))
    password = forms.CharField(label="密码", max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control'}))

def login(no):
    cursor=connections['default'].cursor()
    if(len(str(no))==10):
        sql='select 姓名 from student where  学号=%d'
        cursor.execute(sql%int(no))
        result=cursor.fetchall()
        if len(result)==0:
            return ['学生','学生']
        else:
            return [result[0][0],'学生']
    elif(len(str(no))==8):
        #老师
        sql='select 姓名,身份 from teacher where  工号=%d'
        cursor.execute(sql%int(no))
        result=cursor.fetchall()
        res=list(result[0])
        if len(result)==0:
            return ['学生','学生']
        else:
            return res
    else:
        return ['学生','学生']

def get_classname():
    cur=connections['default'].cursor()
    classname=[]
    cur.execute('select 班级名 from class')
    result=(cur.fetchall())
    for i in result:
        classname.append(i[0])
    return classname
def get_student():
    cur=connections['default'].cursor()
    student_no=[]
    cur.execute('select 学号 from student')
    result=(cur.fetchall())
    for i in result:
        student_no.append(i[0])
    return student_no

def insert_teacher(file_path):
    cur=connections['default'].cursor()

    class_list=['班级号','班级名','班主任工号']
    gd=xlrd.open_workbook(file_path)#"C:\\Users\\ivyru\\Desktop\\class.xlsx")#file_path)#地址#"C:\\Users\\ivyru\\Desktop\\class.xlsx")
    sheet = gd.sheet_by_index(0)
       # 总行数
    rows = sheet.nrows
     # 获取列(经常读取到的excel可能存在空白行或者空白列，这里根据第一行的数据获取要导入的数据的列数)
    rowlsts = [i for i in sheet.row_values(0) if i != '']
    teacher_list=['工号','姓名','身份','班级']
    classname_list=get_classname()
    #print(classname_list)
    cur.execute('select count(*) from class')
    result=cur.fetchall()
    k=result[0][0]
    
    n=0
    nall=0
    for i in range(1,rows):
        try:
            item = sheet.row_values(i)[:len(rowlsts)]
            item[3]=''.join(item[3].split())
            if item[2]=='班主任' and item[3] not in classname_list:
                value=[]
                value.append(k)#班级号

                classname_list.append(item[3])
                value.append(item[3])#班级名
                value.append(item[0])#班主任工号
                k+=1
                data=''
                data2=''
                # 组装参数化sql，通过拼接%s得到占位符
                for j in range(0, len(value)):
                    data += "%s,"
                data = data.rstrip(',')
                for j in range(0, len(item)):
                    data2 += "%s,"
                data2 = data2.rstrip(',')
                if n == 0:
                    #print(value)
                    #print(data)
                    sql2 = "insert into class(%s) values( %s )" % (str.join(',', class_list), data)
                    cur.execute(sql2, tuple(value))

                else:
                    cur.execute(sql2, tuple(value))

            elif item[2]=='班主任' and item[3] in classname_list:#如果已经有该班级 则修改对应的班主任工号
                cur.execute('update class set 班主任工号={0} where 班级名="{1}"'.format(item[0],item[3]))

            if nall == 0:
                #sql = "insert into teacher(%s) values( %s )" % (str.join(',', teacher_list), data2)
                #cur.execute(sql, tuple(item))
                #如果没有就插入 如果有就修改姓名身份班级
                sql='INSERT INTO teacher({0})  VALUES {1} \
                    ON duplicate KEY UPDATE \
                    姓名="{2}",身份="{3}",班级="{4}";'#.format(str.join(',', teacher_list), tuple(item),item[1],item[2],item[3])
                cur.execute(sql.format(str.join(',', teacher_list), tuple(item),item[1],item[2],item[3]))
                nall+=1
            else:
                #print(item)
                #print(sql)
                #cur.execute(sql, tuple(item))
                cur.execute(sql.format(str.join(',', teacher_list), tuple(item),item[1],item[2],item[3]))
        except Exception as ex:
            print(ex)
            return 0
    return 1

def insert_value(typ,file_path):#rows,rowlsts,sheet):#typ可取：核心/综合/2018-2019等年份 #
    cur=connections['default'].cursor()
    gd=xlrd.open_workbook(file_path)#地址#"C:\\Users\\ivyru\\Desktop\\test.xlsx")
    sheet = gd.sheet_by_index(0)
    # 总行数
    rows = sheet.nrows
    # 获取列(经常读取到的excel可能存在空白行或者空白列，这里根据第一行的数据获取要导入的数据的列数)
    rowlsts = [i for i in sheet.row_values(0) if i != '']
    
    gradeall_list=['学号','名次','总学分','总学分绩点','平均学分绩','总门数','优','良','中','及格','不及格','类型','导入时间']
    student_list=['学号','姓名','院系','专业','班级号']
    class_list=['班级号','班级名','班主任工号']
    time=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    n=0
    
    for i in range(1,rows):
        try:
                studentno_list=get_student()#现在的学生姓名
                item =[str(x) for x in sheet.row_values(i)[:len(rowlsts)]]

                value=[]
                for k in range(len(item)):
                    item[k]=''.join(item[k].split())
                intk=int(float(item[1]))                        
                item[1]=str(intk)
                
                for k in range(1,5):
                    value.append(item[k])
                item[5]=''.join(item[5].split())
                #value.append(classname_list.index(item[5]))
                cur.execute("select 班级 from  dualdegree where 学号={0}".format(item[1]))
                res=cur.fetchall()
                if(len(res)==0):
                    cur.execute("select 班级号 from class where 班级名='"+item[5]+"'" )
                    res2=cur.fetchall()
                    if (len(res2)!=0):
                        value.append(res2[0][0])        
                    else:
                            cur.execute('select count(*) from class')
                            result=cur.fetchall()
                            k=result[0][0]
                            class_new=[]
                            class_list=['班级号','班级名','班主任工号']
                            class_new.append(k)
                            class_new.append(item[5])
                            class_new.append(-1)
                            value.append(k)
                            try:
                                sql2 = "insert into class(%s) values( %s )" % (str.join(',', class_list), "%s,%s,%s")
                                cur.execute(sql2, tuple(class_new))
                            except Exception as ex:
                                print(ex)
                                return 0
                else:
                    cur.execute('select c.班级号 from class c,dualdegree d where c.班级名=d.班级')
                    result=cur.fetchall()
                    sql='update student,dualdegree set student.班级号={0}  where student.学号=dualdegree.学号'
                    cur.execute(sql.format(result[0][0]))
                    continue
                        
                data=''
                # 组装参数化sql，通过拼接%s得到占位符
                for j in range(0, len(value)):
                    data += "%s,"
                data = data.rstrip(',')
                #print(studentno_list)
                if int(item[1]) not in studentno_list:#如果没有就插入 如果有就修改原来的
                    
                    
                    if n == 0:
                        sql = "insert into student(%s) values( %s )" % (str.join(',', student_list), data)
                        cur.execute(sql, tuple(value))
                        n+=1
                    else:
                        cur.execute(sql, tuple(value))
                else:
                    cur.execute('update student set 姓名="{0}",院系="{1}",专业="{2}",班级号="{3}"\
                    where 学号={4}'.format(value[1],value[2],value[3],value[4],value[0]))
        except Exception as ex:
            #print("student error")
            print(ex)
            return 0
        
    #成绩表
    n=0
    for i in range(1, rows):
        try:
            item =[str(x) for x in sheet.row_values(i)[:len(rowlsts)]]
            item.append(typ)
            mid=[]
            for k in range(8,10):
                item[k]=''.join(item[k].split())
            for i in range(len(item)):
                
                if(i==1 or i>=6):                
                    if(i==6 or i==7 or i in range(10,16)):                       
                        k=int(float(item[i]))                        
                        mid.append(str(k))
                    else:
                #elif(i==1 or i>=6):
                        mid.append(item[i])
            mid.append(time)
            #print(mid)
            
            data=''
            # 组装参数化sql，通过拼接%s得到占位符
            for j in range(0, len(mid)):
                data += "%s,"
            data = data.rstrip(',')

            
            if n == 0:
                sql='INSERT INTO gradeall({0}) VALUES({1},AES_ENCRYPT("{3}","{2}"),AES_ENCRYPT("{4}","{2}"),AES_ENCRYPT("{5}","{2}"),\
                        AES_ENCRYPT("{6}","{2}"),AES_ENCRYPT("{7}","{2}"),AES_ENCRYPT("{8}","{2}"),AES_ENCRYPT("{9}","{2}"),\
                        AES_ENCRYPT("{10}","{2}"),AES_ENCRYPT("{11}","{2}"),AES_ENCRYPT("{12}","{2}"),"{13}","{14}")\
                        ON duplicate KEY UPDATE \
                        名次=AES_ENCRYPT("{15}","{2}"),总学分=AES_ENCRYPT("{16}","{2}"),总学分绩点=AES_ENCRYPT("{17}","{2}"),\
                        平均学分绩=AES_ENCRYPT("{18}","{2}"),\
                        总门数=AES_ENCRYPT("{19}","{2}"),优=AES_ENCRYPT("{20}","{2}"),良=AES_ENCRYPT("{21}","{2}"),\
                        中=AES_ENCRYPT("{22}","{2}"),\
                        及格=AES_ENCRYPT("{23}","{2}"),不及格=AES_ENCRYPT("{24}","{2}"),导入时间="{25}";'
                
               
                cur.execute(sql.format(str.join(',', gradeall_list), mid[0],key,mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],\
                                      mid[8],mid[9],mid[10],mid[11],mid[12],mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],\
                                      mid[8],mid[9],mid[10],mid[12]))
            else:
                cur.execute(sql.format(str.join(',', gradeall_list), mid[0],key,mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],\
                                      mid[8],mid[9],mid[10],mid[11],mid[12],mid[1],mid[2],mid[3],mid[4],mid[5],mid[6],mid[7],\
                                      mid[8],mid[9],mid[10],mid[12]))

        except Exception as ex:
            print(ex)
            return 0

        finally:
            n += 1
    return 1

def insert_degree(file_path):
    cur=connections['default'].cursor()
    gd=xlrd.open_workbook(file_path)#地址#"C:\\Users\\ivyru\\Desktop\\test.xlsx")
    sheet = gd.sheet_by_index(0)
    # 总行数
    rows = sheet.nrows
    n=0
    # 获取列(经常读取到的excel可能存在空白行或者空白列，这里根据第一行的数据获取要导入的数据的列数)
    rowlsts = [i for i in sheet.row_values(0) if i != '']
    degree_list=['学号','班级']
    for i in range(1,rows):
        try:
            item = sheet.row_values(i)[:len(rowlsts)]
            item[1]=''.join(item[1].split())
                        
            if n == 0:
                sql='INSERT INTO dualdegree({0})  VALUES {1} \
                ON duplicate KEY UPDATE \
                班级="{2}";'
                cur.execute(sql.format(str.join(',', degree_list),tuple(item),item[1]))
                n+=1
            else:
                cur.execute(sql.format(str.join(',', degree_list),tuple(item),item[1]))
                    
            cur.execute('select d.学号,d.班级 from student s,dualdegree d where s.学号=d.学号 and s.学号={0}'.format(item[0]))
            res=cur.fetchall()
            if(len(res)!=0):
                cur.execute('select c.班级号 from class c, dualdegree d where c.班级名="{0}"'.format(item[1]))
                res=cur.fetchall()
                sql2='update student,dualdegree set student.班级号={0}  where student.学号={1}'
                cur.execute(sql2.format(res[0][0],item[0]))
                print(res[0][0])
        except Exception as ex:
            print(ex)   
            return 0
    return 1

def ask_question(askno,askname,title,content,imgpath,parent=-1):
    cur2=connections['default'].cursor()
    cur2.execute('select count(*) from question')
    result=cur2.fetchall()
    qno=result[0][0]
    time=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    if(imgpath==""):
        lquestion=[qno,title,content,askno,askname]
        lquestion.append(time)
        
        if(parent>=0):
            question_list=['qno,title,content,askno,askname,time,parent']
            lquestion.append(parent)
            sql = "insert into question(%s) values( %s )" % (str.join(',', question_list), "%s,%s,%s,%s,%s,%s,%s")
            cur2.execute(sql, tuple(lquestion))
            cur2.execute('update question set child={0} where qno={1}'.format(qno,parent))
        else:
            question_list=['qno,title,content,askno,askname,time']
            sql = "insert into question(%s) values( %s )" % (str.join(',', question_list), "%s,%s,%s,%s,%s,%s")
            cur2.execute(sql, tuple(lquestion))
    else:
        fin=open(imgpath,'rb')
        img=fin.read()
        fin.close()
        lquestion=[qno,title,content,askno,askname,img]
        lquestion.append(time)

        if(parent>=0):
            question_list=['qno,title,content,askno,askname,pic,time,parent']
            lquestion.append(parent)
            sql = "insert into question(%s) values( %s )" % (str.join(',', question_list), "%s,%s,%s,%s,%s,%s,%s,%s")
            cur2.execute(sql, tuple(lquestion))
            cur2.execute('update question set child={0} where qno={1}'.format(qno,parent))
        else:
            question_list=['qno,title,content,askno,askname,pic,time']
            sql = "insert into question(%s) values( %s )" % (str.join(',', question_list), "%s,%s,%s,%s,%s,%s,%s")
            cur2.execute(sql, tuple(lquestion))

def ans_question(ansno,ansname,content,imgpath,public,qno):
    cur2=connections['default'].cursor()
    cur2.execute('select count(*) from answer')
    result=cur2.fetchall()
    ano=result[0][0]
    time=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    if(imgpath==""):
        lanswer=[ano,content,ansno,ansname,public]
        lanswer.append(time)
        answer_list=['ano,content,ansno,ansname,public,time']
        sql = "insert into answer(%s) values( %s )" % (str.join(',', answer_list), "%s,%s,%s,%s,%s,%s")
        cur2.execute(sql, tuple(lanswer))
    else:
        fin=open(imgpath,'rb')
        img=fin.read()
        fin.close()

        lanswer=[ano,content,ansno,ansname,public,img]
        lanswer.append(time)
        answer_list=['ano,content,ansno,ansname,public,pic,time']
        sql = "insert into answer(%s) values( %s )" % (str.join(',', answer_list), "%s,%s,%s,%s,%s,%s,%s")
        cur2.execute(sql, tuple(lanswer))
    
    #加入问答表
    cur2.execute('select count(*) from quesans')
    result=cur2.fetchall()
    number=result[0][0]
    lquesans=[number,qno,ano]
    quesans_list=['number,qno,ano']
    sql = "insert into quesans(%s) values( %s )" % (str.join(',', quesans_list), "%s,%s,%s")
    cur2.execute(sql, tuple(lquesans))


def all_parent_question(qno):
    cur2=connections['default'].cursor()
    sql='select parent from question where qno={0}'
    cur2.execute(sql.format(qno))
    result=cur2.fetchall()
    parent_ques=[]
    while(result[0][0]!=None):
        parent_ques.append(result[0][0])
        cur2.execute(sql.format(result[0][0]))
        result=cur2.fetchall()
    return sorted(parent_ques)

def all_child_question(qno):
    cur2=connections['default'].cursor()
    sql='select child from question where qno={0}'
    cur2.execute(sql.format(qno))
    result=cur2.fetchall()
    child_ques=[]
    while(result[0][0]!=None):
        child_ques.append(result[0][0])
        cur2.execute(sql.format(result[0][0]))
        result=cur2.fetchall()
    return sorted(child_ques)
def show_question(qno):
#输入：qno是问题编
#输出：qno的所有父问题及所有子问题
    parent=all_parent_question(qno)
    child=all_child_question(qno)
    all_question=parent
    all_question.append(qno)
    for k in child:
        all_question.append(k)
    return all_question

#添加每一个标签
def add_tags(tag,my_Qno):
    cur2=connections['default'].cursor()
    #首先在tag表中添加
    sql1 = "select Tno,citations from tags where tag_name = %s "% ("%s")
    cur2.execute(sql1,tag)
    result=cur2.fetchall()
    print(result)
    if result:
        print("tag表中已经存在此标签")
        my_Tno = result[0][0]
        my_citations = result[0][1]
        
        sql7 = "update tags set citations = {0} where Tno = {1}".format(my_citations+1,my_Tno)#更新引用次数
        cur2.execute(sql7)
    else:   
        sql2 = "INSERT INTO tags (tag_name) VALUES (%s)" % ( "%s")
        cur2.execute(sql2,tag)
        
        sql8 = "select Tno from tags where tag_name = %s "% ("%s")
        cur2.execute(sql8,tag)
        result4=cur2.fetchall()
        my_Tno = result4[0][0]
        
        sql6 = "update tags set citations = {0} where Tno = {1}".format(1,my_Tno)#更新引用次数
        cur2.execute(sql6)
        
    sql4 = "select Tno from QuesTags where Tno = {0} and Qno = {1} ".format(my_Qno,my_Tno)
    cur2.execute(sql4)
    result3=cur2.fetchall()
    if result3:
        print("questag表中已经存在此标签")
    else:
        sql5 = "INSERT INTO QuesTags (Qno, Tno) VALUES ({0},{1})".format(my_Qno,my_Tno)
        cur2.execute(sql5)

def QAchange(changemethod,changeid,changecontent):
    cur2=connections['default'].cursor()
    if(changemethod==1):#问题标题
        sql='update question set title="{0}" where qno={1}'
        cur2.execute(sql.format(changecontent,changeid))
    elif(changemethod==2):
        sql='update question set content="{0}" where qno={1}'
        cur2.execute(sql.format(changecontent,changeid))
    elif(changemethod==3):
        sql='update answer set content="{0}" where ano={1}'
        cur2.execute(sql.format(changecontent,changeid))

def upload_file(request):
    myFile=request.FILES.get("file", None) # 获取上传的文件，如果没有文件，则默认为None 
    if not myFile:
        return ""
    s = os.getcwd()+'/templates/files/'
    destination = open(os.path.join(s,myFile.name),'wb+')  #打开特定的文件进行二进制的写操作 
    for chunk in myFile.chunks():   # 分块写入文件 
        destination.write(chunk)
    destination.close()

    return os.path.join(s,myFile.name)

def vruc_login(request):
    loginServer = "http://cas.ruc.edu.cn/cas/login"
    myurl = "http://127.0.0.1:8080/cas"
    return redirect(loginServer+'?service='+myurl,code=302)

def cas(request):
    ticket = request.GET.get("ticket")
    if ticket:
        myurl = "http://127.0.0.1:8080/cas"
        validateServer = "http://cas.ruc.edu.cn/cas/serviceValidate"
        validateurl = validateServer+"?ticket="+ticket+'&service='+myurl
        dom = requests.get(validateurl).text
        soup = BeautifulSoup(dom,'lxml')
        sno = soup.find('sso:user').string
        name = soup.find('sso:attribute',{'name':'sn'})['value']
        power=login(sno)
        request.session['is_login'] = True
        request.session['user_id'] = int(sno)
        request.session['user_name']= name
        request.session['user_power']=power[1]

        if(power[1] == '答疑老师'):
            return redirect('qateacher_qa1')
        elif(power[1] == '班主任'):
            return redirect('headteacher_check1')
        elif(power[1] == '学院老师'):
            return redirect('institution')
        elif(power[1] == '教秘'):
            return redirect('manager')
        else:
            return redirect('student')
    return HttpResponse("微人大验证错误！")

def index(request):
    context={}

    if request.session.get('is_login',None):
        power = request.session['user_power']
        if(power == '答疑老师'):
            return redirect('qateacher_qa1')
        elif(power == '班主任'):
            return redirect('headteacher_check1')
        elif(power == '学院老师'):
            return redirect('institution')
        elif(power == '教秘'):
            return redirect('manager')
        else:
            return redirect('student')

    if request.method == "POST":
        login_form = UserForm(request.POST)
        if login_form.is_valid():
            username = login_form.cleaned_data['username']
            power=login(username)
            request.session['is_login'] = True
            request.session['user_id'] = int(username)
            request.session['user_name']=power[0]
            request.session['user_power']=power[1]

            if(power[1] == '答疑老师'):
                return redirect('qateacher_qa1')
            elif(power[1] == '班主任'):
                return redirect('headteacher_check1')
            elif(power[1] == '学院老师'):
                return redirect('institution')
            elif(power[1] == '教秘'):
                return redirect('manager')
            else:
                return redirect('student')

    return render(request,'index.html',context)

def log_out(request):
    request.session.flush()
    return redirect('index')

def manager(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}
    return render(request,'z_manager/manager.html',context)

def manager_check1(request):#教秘查个人
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型  from gradeall where 类型 !="所有" and 类型 !="核心" '
    #curtest.execute("select id,AES_DECRYPT(name,'123'),CONVERT(AES_DECRYPT(class,'123') USING utf8) 
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        sno = int(request.POST['studentnumber'])       #这是学号
        print(sno)
        if choice =='3':
            print("ok")
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号={0}\
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(sno,year,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_manager/manager-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "{3}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key,year)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #print("***************")
            #print(raw[0])
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
                       #查询的结果
            context["select_answer"]=raw1
            
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(sno,key) #应该改成所有
            cursor.execute(sql)
            
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_manager/manager-check-2.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号={0} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(sno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_manager/manager-check-3.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1


    return render(request,'z_manager/manager-check-1.html',context)


def manager_check2(request):#教秘查班级
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    cursor=connections['default'].cursor()
    sql='select distinct 班级名 from class order by 班级名'
    cursor.execute(sql)
    class_list=cursor.fetchall()
    classes=[]
    for i in class_list:
        for j in i:
            classes.append(j)
    context['class_list'] = classes
    
    if request.method == "POST":
        print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        classname = request.POST['class_selected']       #这是班级名
        print(classname)
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(classname,year,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            
            for x in raw:
                print(x)
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')
        
            context['select_answer'] = raw
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(classname,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
    
            rank = 0
            label = 0
            count = 0
    
            for x in raw:
                print(x)
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(classname,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            for x in raw:
                print(x)
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
    

    return render(request,'z_manager/manager-check-2.html',context)

def manager_check3(request): #教秘查所有人
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        #print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{0}" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(year,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
        
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                rank = 0
                label = 0
                count = 0
            
    
                    
                for x in ans: #x是一个相同班级的字典
                    
                    if x['名次'] == label: #年级排名相同的情况
                        class_k.append(rank)
                        count = count + 1
                    else:
                        rank = rank+count+1
                        count=0
                        class_k.append(rank)
                        label = x['名次']
              
                
            print(len(class_k))
            print(len(raw))
            i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
            
            raw=sorted(raw,key = itemgetter('学号'))
            context['select_answer'] = raw
        elif choice =='2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{0}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{0}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{0}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{0}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{0}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{0}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{0}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{0}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{0}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED)'.format(key)#应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                rank = 0
                label = 0
                count = 0
                    
                for x in ans: #x是一个相同班级的字典
                    
                    if x['名次'] == label: #年级排名相同的情况
                        class_k.append(rank)
                        count = count + 1
                    else:
                        rank = rank+count+1
                        count=0
                        class_k.append(rank)
                        label = x['名次']
    
            print(len(class_k))
            print(len(raw))
            i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
                
            raw=sorted(raw,key = itemgetter('学号'))
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
            
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{0}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{0}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{0}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{0}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{0}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{0}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{0}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{0}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{0}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED)'.format(key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                rank = 0
                label = 0
                count = 0
                for x in ans: #x是一个相同班级的字典
                    
                    if x['名次'] == label: #年级排名相同的情况
                        class_k.append(rank)
                        count = count + 1
                    else:
                        rank = rank+count+1
                        count=0
                        class_k.append(rank)
                        label = x['名次']
            i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
            raw=sorted(raw,key = itemgetter('学号'))
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
    return render(request,'z_manager/manager-check-3.html',context)


def manager_input1(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理 
        path=upload_file(request)
        flag = insert_value("所有",path)
        if flag==1:
            messages.success(request, '提交成功！')
        else:
            messages.success(request, '提交失败！请检查格式是否与要求相符')
        return redirect('manager_input1')
    return render(request,'z_manager/manager-input-1.html',context)


def manager_input2(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理 
        path=upload_file(request)
        flag = insert_value("核心",path)
        if flag==1:
            messages.success(request, '提交成功！')
        else:
            messages.success(request, '提交失败！请检查格式是否与要求相符')
        return redirect('manager_input2')
    return render(request,'z_manager/manager-input-2.html',context)


def manager_input3(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理 
        year = request.POST['year_decide']
        path=upload_file(request)
        flag = insert_value(year,path)
        if flag==1:
            messages.success(request, '提交成功！')
        else:
            messages.success(request, '提交失败！请检查格式是否与要求相符')
        return redirect('manager_input3')
    return render(request,'z_manager/manager-input-3.html',context)


def manager_input4(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理
        path=upload_file(request)
        flag = insert_teacher(path)
        if flag==1:
            messages.success(request, '提交成功！')
        else:
            messages.success(request, '提交失败！请检查格式是否与要求相符')
        return redirect('manager_input4')
    return render(request,'z_manager/manager-input-4.html',context)

def manager_input5(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')

    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理
        path=upload_file(request)
        flag = insert_degree(path)
        if flag==1:
            messages.success(request, '提交成功！')
        else:
            messages.success(request, '提交失败！请检查格式是否与要求相符')
        return redirect('manager_input5')
    return render(request,'z_manager/manager-input-5.html',context)

######################学院老师###################################
def institution(request):
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    return render(request,'z_institution-teacher/institutionteacher.html',context)

def institution_check1(request):#学院老师查个人
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        sno = int(request.POST['studentnumber'])       #这是学号
        print(sno)
        if choice =='3':
            print("ok")
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号={0} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(sno,year,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_institution-teacher/institutionteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "{3}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key,year)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #print("***************")
            #print(raw[0])
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
                       #查询的结果
            context["select_answer"]=raw1
            
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(sno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_institution-teacher/institutionteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号={0} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(sno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_institution-teacher/institutionteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1


    return render(request,'z_institution-teacher/institutionteacher-check-1.html',context)


def institution_check2(request):#学院老师查班级
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    cursor=connections['default'].cursor()
    sql='select distinct 班级名 from class order by 班级名'
    cursor.execute(sql)
    class_list=cursor.fetchall()
    classes=[]
    for i in class_list:
        for j in i:
            classes.append(j)
    context['class_list'] = classes
    
    if request.method == "POST":
        print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        classname = request.POST['class_selected']       #这是班级名
        print(classname)
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(classname,year,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')
        
            context['select_answer'] = raw
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(classname,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
    
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名="{0}" \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(classname,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
    

    return render(request,'z_institution-teacher/institutionteacher-check-2.html',context)

def institution_check3(request): #学院老师查所有人
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        #print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{0}" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(year,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
        
            #result=cursor.fetchall()
            #print(result)
            #result2=list(result)
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                for k in ans:
                    rank=ans.index(k)
                    #k=list(k)
                    #k.append(rank+1)
                    class_k.append(rank+1)
                    #print(k)
              
                
            print(len(class_k))
            print(len(raw))
            i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
            
            raw=sorted(raw,key = itemgetter('学号'))
            context['select_answer'] = raw
        elif choice =='2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{0}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{0}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{0}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{0}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{0}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{0}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{0}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{0}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{0}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED)'.format(key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            #result=cursor.fetchall()
            #print(result)
            #result2=list(result)
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                for k in ans:
                    rank=ans.index(k)
                    #k=list(k)
                    #k.append(rank+1)
                    class_k.append(rank+1)
                    #print(k)
    
            print(len(class_k))
            print(len(raw))
            i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
                
            raw=sorted(raw,key = itemgetter('学号'))
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
            
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{0}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{0}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{0}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{0}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{0}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{0}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{0}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{0}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{0}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 \
                        order by  c.班级名,cast(CONVERT(AES_DECRYPT(g.名次,"{0}") USING utf8) as SIGNED)'.format(key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            data_2=groupby(raw,itemgetter('班级名'))
            class_k=[]
            for i,v in data_2:
                #print(v)
                ans=list(v)
                #print(ans)
                for k in ans:
                    rank=ans.index(k)
                    #k=list(k)
                    #k.append(rank+1)
                    class_k.append(rank+1)
                    #print(k)
                    
                    #print(len(class_k))
                    #print(len(raw))
                    i = 0
            for x in raw:
                x['班级名次'] = class_k[i]
                i = i + 1
            raw=sorted(raw,key = itemgetter('学号'))
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
    return render(request,'z_institution-teacher/institutionteacher-check-3.html',context)


######################班主任##################################
def headteacher_check1(request): #班主任查个人
    power = request.session['user_power']
    if(power != '班主任'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        #print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        tno = int(request.session['user_id'])
        sno = int(request.POST['studentnumber'])
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{3}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{3}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{3}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{3}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{3}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{3}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{3}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{3}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{3}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{3}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号={0} and c.班主任工号 ={2} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{3}") USING utf8) as SIGNED)'.format(sno,year,tno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_head_teacher/headteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "{3}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key,year)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #print("***************")
            #print(raw[0])
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
                       #查询的结果
            context["select_answer"]=raw1
        
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班主任工号= {0} and s.学号 = {1} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(tno,sno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_head_teacher/headteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1
            
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c\
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班主任工号= {0} and s.学号 = {1} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(tno,sno,key) #应该改成所有
            cursor.execute(sql)
            raw1=dictfetchall(cursor)
            

            if(len(raw1)==0):
                return render(request,'z_head_teacher/headteacher-check-1.html',context)
    
            #查找班级排名
            sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
            cursor.execute(sql2)
            raw2=dictfetchall(cursor)
            
            rank = 1
            label = raw1[0]['名次']
            for x in raw2:
                if x['名次'] < label: 
                    rank = rank + 1
           
            raw1[0]['班级名次'] = rank
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw1)
            print('查询完毕')
            #查询的结果
            context["select_answer"]=raw1
            
    return render(request,'z_head_teacher/headteacher-check-1.html',context)
    
def headteacher_check2(request): #班主任查班级内排名
    power = request.session['user_power']
    if(power != '班主任'):
        return HttpResponse('页面打开错误！')

    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years

    if request.method == "POST":
        #print(request.POST)
        choice = request.POST['choice']     #1代表综合排名，2代表核心排名，3代表学年排名
        tno = int(request.session['user_id'])
        if choice =='3':
            year = request.POST['selected_year']
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{2}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{2}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{2}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{2}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{2}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班主任工号 ={0} \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED)'.format(tno,year,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
    
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')
            
            raw=sorted(raw,key = itemgetter('学号'))
        
            context['select_answer'] = raw
        
        elif choice == '2':
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班主任工号= {0}\
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(tno,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
                
            
            raw=sorted(raw,key = itemgetter('学号'))
            
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
            
        else:
            cursor=connections['default'].cursor()
            sql='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
                        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
                        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
                        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,CONVERT(AES_DECRYPT(g.优,"{1}") USING utf8) 优,\
                        CONVERT(AES_DECRYPT(g.良,"{1}") USING utf8) 良,CONVERT(AES_DECRYPT(g.中,"{1}") USING utf8) 中,\
                        CONVERT(AES_DECRYPT(g.及格,"{1}") USING utf8) 及格,CONVERT(AES_DECRYPT(g.不及格,"{1}") USING utf8) 不及格,g.导入时间\
                        from gradeall g, student s , class c \
                        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班主任工号= {0}  \
                        order by cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED)'.format(tno,key) #应该改成所有
            cursor.execute(sql)
            raw=dictfetchall(cursor)
            
            rank = 0
            label = 0
            count = 0
            for x in raw:
                if x['名次'] == label: #年级排名相同的情况
                    x['班级名次'] = rank
                    count = count + 1
                else:
                    rank = rank+count+1
                    count=0
                    x['班级名次'] = rank
                    label = x['名次']
            
            raw=sorted(raw,key = itemgetter('学号'))
            #sql='select from class c, student s'
            print('查询结果为：')
            print(raw)
            print('查询完毕')

            context['select_answer'] = raw
            
    return render(request,'z_head_teacher/headteacher-check-2.html',context)

#############################学生####################
def student(request):
    context={}
    return render(request,'z_student/student.html',context)

def student_check1(request):
    context = {}

    sno = int(request.session['user_id'])
    cursor=connections['default'].cursor()
    sql1='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,g.导入时间 \
        from gradeall g, student s , class c \
        where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号 ={0}'.format(sno,key) #应该改成所有
    cursor.execute(sql1)
    raw1=dictfetchall(cursor)
    if(len(raw1)==0):
        return render(request,'z_student/student-check-1.html',context)
    #查找班级排名
    sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "所有" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
    cursor.execute(sql2)
    raw2=dictfetchall(cursor)
    
    rank = 1
    label = raw1[0]['名次']
    for x in raw2:
        if x['名次'] < label: 
            rank = rank + 1
           
    raw1[0]['班级名次'] = rank
    
    print('查询结果为：')
    print(raw1)
    print('查询完毕')

    context['select_answer'] = raw1
    
    return render(request,'z_student/student-check-1.html',context)

def student_check2(request):
    context = {}

    sno = int(request.session['user_id'])
    cursor=connections['default'].cursor()
    sql1='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
        cast(CONVERT(AES_DECRYPT(g.名次,"{1}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{1}") USING utf8) 总学分,\
        CONVERT(AES_DECRYPT(g.总学分绩点,"{1}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{1}") USING utf8) 平均学分绩,\
        CONVERT(AES_DECRYPT(g.总门数,"{1}") USING utf8) 总门数,g.导入时间 \
        from gradeall g, student s , class c \
        where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号 ={0}'.format(sno,key) #应该改成所有
    cursor.execute(sql1)
    raw1=dictfetchall(cursor)

    if(len(raw1)==0):
        return render(request,'z_student/student-check-2.html',context)
    
    #查找班级排名
    sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "核心" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],key)
    cursor.execute(sql2)
    raw2=dictfetchall(cursor)
    
    rank = 1
    label = raw1[0]['名次']
    for x in raw2:
        if x['名次'] < label: 
            rank = rank + 1
           
    raw1[0]['班级名次'] = rank
    
    print('查询结果为：')
    print(raw1)
    print('查询完毕')


    context['select_answer'] = raw1
    return render(request,'z_student/student-check-2.html',context)

def student_check3(request):
    context={}
    cursor=connections['default'].cursor()
    sql='select distinct 类型 from gradeall where 类型!="所有" and 类型!="核心" '
    cursor.execute(sql)
    sno_year=cursor.fetchall()
    years=[]
    for i in sno_year:
        for j in i:
            years.append(j)
    context['year_list'] = years
    
    if request.method == "POST":
        sno = int(request.session['user_id'])
        year = request.POST['selected_year']
        cursor=connections['default'].cursor()
        sql1='select s.学号,s.姓名,s.院系,s.专业,c.班级名,\
        cast(CONVERT(AES_DECRYPT(g.名次,"{2}") USING utf8) as SIGNED) 名次,CONVERT(AES_DECRYPT(g.总学分,"{2}") USING utf8) 总学分,\
        CONVERT(AES_DECRYPT(g.总学分绩点,"{2}") USING utf8) 总学分绩点,CONVERT(AES_DECRYPT(g.平均学分绩,"{2}") USING utf8) 平均学分绩,\
        CONVERT(AES_DECRYPT(g.总门数,"{2}") USING utf8) 总门数,g.导入时间 \
        from gradeall g, student s , class c \
        where 类型 = "{1}" and g.学号=s.学号 and c.班级号=s.班级号 and s.学号 ={0}'.format(sno,year,key) #应该改成所有
        cursor.execute(sql1)
        raw1=dictfetchall(cursor)

        if(len(raw1)==0):
            render(request,'z_student/student-check-3.html',context)
        
        sql2='select s.学号,cast(CONVERT(AES_DECRYPT(g.名次,"{3}") USING utf8) as SIGNED) 名次 from gradeall g, student s , class c where 类型 = "{2}" and g.学号=s.学号 and c.班级号=s.班级号 and c.班级名 ="{0}" and s.学号!={1}'.format(raw1[0]['班级名'],raw1[0]['学号'],year,key)
        cursor.execute(sql2)
        raw2=dictfetchall(cursor)
        
        rank = 1
        label = raw1[0]['名次']
        for x in raw2:
            if x['名次'] < label: 
                rank = rank + 1
           
        raw1[0]['班级名次'] = rank
    
        print('查询结果为：')
        print(raw1)
        print('查询完毕')
        
        context['select_answer'] = raw1
    
    return render(request,'z_student/student-check-3.html',context)

def student_qa1(request):
    context={}
    cur2=connections['default'].cursor()

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    oldtags=dictfetchall(cur2)
    context['oldtags']=oldtags

    if request.method == "POST":
        search_tag=0
        if 'selected_label' in request.POST and (request.POST['selected_label'] != '选择标签'):
            search_tag = request.POST['selected_label']
        else:
            for tag in oldtags:
                if tag['tag_name'] in request.POST:
                    search_tag = tag['tag_name']
        if search_tag==0:
            #不是标签，要搜索内容
            key = "%"+request.POST['search']+"%"
            sql='select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                from question q,quesans qa,answer a where (q.title like "{0}" or q.content like "{1}")\
                and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(key,key)
            cur2.execute(sql)
            res=dictfetchall(cur2)
        else:
            #是标签，搜索标签
            sql1 = "select Tno from tags where tag_name = '{0}'".format(search_tag)
            cur2.execute(sql1)
            mid=cur2.fetchall()
            my_Tno = mid[0][0]

            sql2 = "select Qno from questags where Tno = {0}".format(my_Tno)
            cur2.execute(sql2)
            result=cur2.fetchall()
            res=[]

            for i in result:
                sql3 = 'select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                from question q,quesans qa,answer a where q.qno = {0}\
                and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(i[0])
                cur2.execute(sql3)
                temp = dictfetchall(cur2)
                res.extend(temp)
    else:
        cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
        res=dictfetchall(cur2)

    for i in range(len(res)):
        sql1 = "select Tno from questags where Qno = {0}".format(res[i]['qno'])
        cur2.execute(sql1)
        tagnums=cur2.fetchall()
    
        tags = []
        for tagnum in tagnums:
            sql2 = "select tag_name from tags where Tno = {0}".format(tagnum[0])
            cur2.execute(sql2)
            content = cur2.fetchall()
            tags.append(content[0][0])
        res[i]['tags']=tags

    context['publicques']=res

    return render(request,'z_student/student-qa-1.html',context)
def student_qa2(request,qid):
    context={}
    cur2=connections['default'].cursor()

    if request.method == "POST":
        if 'titlechange' in request.POST:
            changemethod = 1
            changeid = request.POST['titlechange']
        else:
            changemethod = 2
            changeid = request.POST['contentchange']

        changecontent = request.POST['answer-content']
        QAchange(changemethod,changeid,changecontent)

    all_question = show_question(qid)
    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question
    context['lastqno']=question[-1]['qno']

    answer=[]
    for i in all_question:
        cur2.execute('select a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
    context['answer']=answer

    return render(request,'z_student/student-qa-2.html',context)

def student_qa3(request):
    context={}
    if request.method == "POST":    # 请求方法为POST时，进行处理 
        path=upload_file(request)
        ask_question(request.session['user_id'],request.session['user_name'],request.POST['question-title'],request.POST['question-content'],path)
        return redirect('student_qa4')
    return render(request,'z_student/student-qa-3.html',context)

def student_qa3plus(request,qid):
    context={}
    context['parentid']=qid

    if request.method == "POST":    # 请求方法为POST时，进行处理 
        path=upload_file(request)
        ask_question(request.session['user_id'],request.session['user_name'],request.POST['question-title'],request.POST['question-content'],path,int(qid))
        return redirect('student_qa4')
    
    cur2=connections['default'].cursor()

    all_question = show_question(qid)
    if(len(all_question)>1):
        context['ischild']=1

    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question

    answer=[]
    for i in all_question:
        cur2.execute('select a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
    context['answer']=answer

    return render(request,'z_student/student-qa-3plus.html',context)

def student_qa4(request):
    context={}
    sno = request.session['user_id']
    cur2=connections['default'].cursor()
    cur2.execute('select Qno,TITLE,time from question where askno={0} order by time DESC'.format(sno))
    ans=cur2.fetchall()
    result=[]
    for i in range(len(ans)):
        result.append(list(ans[i]))
    
    for i in range(len(result)):
        cur2.execute('select * from quesans where ano={0}'.format(result[i][0]))
        res=cur2.fetchall()
        if len(res)==0:
            result[i].append(0)
        else:
            result[i].append(1)
    context['result']=result
    return render(request,'z_student/student-qa-4.html',context)


def institution_qa1(request):
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    cur2=connections['default'].cursor()

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    oldtags=dictfetchall(cur2)
    context['oldtags']=oldtags

    if request.method == "POST":
        search_tag=0
        if 'selected_label' in request.POST and (request.POST['selected_label'] != '选择标签'):
            search_tag = request.POST['selected_label']
        else:
            for tag in oldtags:
                if tag['tag_name'] in request.POST:
                    search_tag = tag['tag_name']
        if search_tag==0:
            if 'cancelpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=0 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['cancelpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            elif 'setpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=1 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['setpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            else:
                #不是标签，要搜索内容
                key = "%"+request.POST['search']+"%"
                sql='select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                    from question q,quesans qa,answer a where (q.title like "{0}" or q.content like "{1}")\
                    and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(key,key)
                cur2.execute(sql)
                res=dictfetchall(cur2)
        else:
            #是标签，搜索标签
            sql1 = "select Tno from tags where tag_name = '{0}'".format(search_tag)
            cur2.execute(sql1)
            mid=cur2.fetchall()
            my_Tno = mid[0][0]

            sql2 = "select Qno from questags where Tno = {0}".format(my_Tno)
            cur2.execute(sql2)
            result=cur2.fetchall()
            res=[]

            for i in result:
                sql3 = 'select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                from question q,quesans qa,answer a where q.qno = {0}\
                and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(i[0])
                cur2.execute(sql3)
                temp = dictfetchall(cur2)
                res.extend(temp)

    else:
        cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
        res=dictfetchall(cur2)

    for i in range(len(res)):
        sql1 = "select Tno from questags where Qno = {0}".format(res[i]['qno'])
        cur2.execute(sql1)
        tagnums=cur2.fetchall()
    
        tags = []
        for tagnum in tagnums:
            sql2 = "select tag_name from tags where Tno = {0}".format(tagnum[0])
            cur2.execute(sql2)
            content = cur2.fetchall()
            tags.append(content[0][0])
        res[i]['tags']=tags

    context['publicques']=res

    cur2.execute('select qno,title,askno,askname,time,parent from question order by time DESC')
    ans=cur2.fetchall()
    result=[]
    for i in range(len(ans)):
        result.append(list(ans[i]))
    
    for i in range(len(result)):
        cur2.execute('select * from quesans where qno={0}'.format(result[i][0]))
        res=cur2.fetchall()
        if len(res)==0:
            result[i].append(0)
        else:
            result[i].append(1)

        sql='select public from answer a,quesans qa where qa.qno={0} and qa.ano=a.ano'
        cur2.execute(sql.format(int(result[i][0])))
        res=dictfetchall(cur2)
        if len(res)==0:
            result[i].append(0)
        elif result[i][5]==None and res[0]['public']==0:
            result[i].append(1)
        else:
            result[i].append(0)

    context['result']=result

    return render(request,'z_institution-teacher/institutionteacher-qa-1.html',context)

def institution_qa2(request,qid):
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')
    
    context={}
    cur2=connections['default'].cursor()

    if request.method == "POST":
        if 'titlechange' in request.POST:
            changemethod = 1
            changeid = request.POST['titlechange']
        elif 'contentchange' in request.POST:
            changemethod = 2
            changeid = request.POST['contentchange']
        else:
            changemethod = 3
            changeid = request.POST['answerchange']

        changecontent = request.POST['answer-content']
        QAchange(changemethod,changeid,changecontent)
    
    all_question = show_question(qid)
    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question
    context['lastqno']=question[-1]['qno']

    context['ifanswer']=1
    answer=[]
    cout=0
    for i in all_question:
        cur2.execute('select a.ano,a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
            if(cout == len(all_question)-1):
                context['ifanswer']=0
        cout=cout+1
    context['answer']=answer
    return render(request,'z_institution-teacher/institutionteacher-qa-2.html',context)

def institution_qa3(request,qid):
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')
    
    context={}
    context['ischild']=0
    cur2=connections['default'].cursor()

    all_question = show_question(qid)
    if(len(all_question)>1):
        context['ischild']=1

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    result=dictfetchall(cur2)
    context['oldtags']=result
    
    if request.method == "POST":
        path=upload_file(request)
        if 'if-public' in request.POST:
            public=1
        else:
            public=0

        for tag in result:
            if tag['tag_name'] in request.POST:
                add_tags(tag['tag_name'],qid)
        if 'newtag' in request.POST and request.POST['newtag'] != "":
            tags = request.POST['newtag'].split("#")
            for tag in tags:
                add_tags(tag,qid)

        ans_question(request.session['user_id'],request.session['user_name'],request.POST['answer-content'],path,public,qid)
        return redirect('institution_qa4')

    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question

    answer=[]
    for i in all_question:
        cur2.execute('select a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
    context['answer']=answer
    return render(request,'z_institution-teacher/institutionteacher-qa-3.html',context)

def institution_qa4(request):
    power = request.session['user_power']
    if(power != '学院老师'):
        return HttpResponse('页面打开错误！')

    context={}
    cur2=connections['default'].cursor()
    cur2.execute('select qno,title,askno,askname,time,parent from question where qno not in (select qno from quesans) order by time DESC')
    ans=dictfetchall(cur2)
    context['result']=ans
    return render(request,'z_institution-teacher/institutionteacher-qa-4.html',context)

def qateacher_qa1(request):
    power = request.session['user_power']
    if(power != '答疑老师'):
        return HttpResponse('页面打开错误！')
    context={}
    cur2=connections['default'].cursor()

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    oldtags=dictfetchall(cur2)
    context['oldtags']=oldtags

    if request.method == "POST":
        search_tag=0
        if 'selected_label' in request.POST and (request.POST['selected_label'] != '选择标签'):
            search_tag = request.POST['selected_label']
        else:
            for tag in oldtags:
                if tag['tag_name'] in request.POST:
                    search_tag = tag['tag_name']
        if search_tag==0:
            if 'cancelpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=0 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['cancelpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            elif 'setpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=1 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['setpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            else:
                #不是标签，要搜索内容
                key = "%"+request.POST['search']+"%"
                sql='select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                    from question q,quesans qa,answer a where (q.title like "{0}" or q.content like "{1}")\
                    and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(key,key)
                cur2.execute(sql)
                res=dictfetchall(cur2)
        else:
            #是标签，搜索标签
            sql1 = "select Tno from tags where tag_name = '{0}'".format(search_tag)
            cur2.execute(sql1)
            mid=cur2.fetchall()
            my_Tno = mid[0][0]

            sql2 = "select Qno from questags where Tno = {0}".format(my_Tno)
            cur2.execute(sql2)
            result=cur2.fetchall()
            res=[]

            for i in result:
                sql3 = 'select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                from question q,quesans qa,answer a where q.qno = {0}\
                and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(i[0])
                cur2.execute(sql3)
                temp = dictfetchall(cur2)
                res.extend(temp)

    else:
        cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
        res=dictfetchall(cur2)

    for i in range(len(res)):
        sql1 = "select Tno from questags where Qno = {0}".format(res[i]['qno'])
        cur2.execute(sql1)
        tagnums=cur2.fetchall()
    
        tags = []
        for tagnum in tagnums:
            sql2 = "select tag_name from tags where Tno = {0}".format(tagnum[0])
            cur2.execute(sql2)
            content = cur2.fetchall()
            tags.append(content[0][0])
        res[i]['tags']=tags

    context['publicques']=res

    cur2.execute('select qno,title,askno,askname,time,parent from question order by time DESC')
    ans=cur2.fetchall()
    result=[]
    for i in range(len(ans)):
        result.append(list(ans[i]))
    
    for i in range(len(result)):
        cur2.execute('select * from quesans where qno={0}'.format(result[i][0]))
        res=cur2.fetchall()
        if len(res)==0:
            result[i].append(0)
        else:
            result[i].append(1)

        sql='select public from answer a,quesans qa where qa.qno={0} and qa.ano=a.ano'
        cur2.execute(sql.format(int(result[i][0])))
        res=dictfetchall(cur2)
        if len(res)==0:
            result[i].append(0)
        elif result[i][5]==None and res[0]['public']==0:
            result[i].append(1)
        else:
            result[i].append(0)

    context['result']=result
    return render(request,'z_Q&A_teacher/qateacher-qa-1.html',context)

def qateacher_qa2(request,qid):
    power = request.session['user_power']
    if(power != '答疑老师'):
        return HttpResponse('页面打开错误！')
    
    context={}
    cur2=connections['default'].cursor()

    if request.method == "POST":
        if 'titlechange' in request.POST:
            changemethod = 1
            changeid = request.POST['titlechange']
        elif 'contentchange' in request.POST:
            changemethod = 2
            changeid = request.POST['contentchange']
        else:
            changemethod = 3
            changeid = request.POST['answerchange']

        changecontent = request.POST['answer-content']
        QAchange(changemethod,changeid,changecontent)
    
    all_question = show_question(qid)
    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question
    context['lastqno']=question[-1]['qno']

    context['ifanswer']=1
    answer=[]
    cout=0
    for i in all_question:
        cur2.execute('select a.ano,a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
            if(cout == len(all_question)-1):
                context['ifanswer']=0
        cout=cout+1
    context['answer']=answer
    return render(request,'z_Q&A_teacher/qateacher-qa-2.html',context)

def qateacher_qa3(request,qid):
    power = request.session['user_power']
    if(power != '答疑老师'):
        return HttpResponse('页面打开错误！')
    
    context={}
    context['ischild']=0
    cur2=connections['default'].cursor()

    all_question = show_question(qid)
    if(len(all_question)>1):
        context['ischild']=1

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    result=dictfetchall(cur2)
    context['oldtags']=result
    
    if request.method == "POST":
        path=upload_file(request)
        if 'if-public' in request.POST:
            public=1
        else:
            public=0

        for tag in result:
            if tag['tag_name'] in request.POST:
                add_tags(tag['tag_name'],qid)
        if 'newtag' in request.POST and request.POST['newtag'] != "":
            tags = request.POST['newtag'].split("#")
            for tag in tags:
                add_tags(tag,qid)

        ans_question(request.session['user_id'],request.session['user_name'],request.POST['answer-content'],path,public,qid)
        return redirect('qateacher_qa4')

    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question

    answer=[]
    for i in all_question:
        cur2.execute('select a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
    context['answer']=answer

    return render(request,'z_Q&A_teacher/qateacher-qa-3.html',context)

def qateacher_qa4(request):
    power = request.session['user_power']
    if(power != '答疑老师'):
        return HttpResponse('页面打开错误！')
    
    context={}
    cur2=connections['default'].cursor()
    cur2.execute('select qno,title,askno,askname,time,parent from question where qno not in (select qno from quesans) order by time DESC')
    ans=dictfetchall(cur2)
    context['result']=ans

    return render(request,'z_Q&A_teacher/qateacher-qa-4.html',context)

def manager_qa1(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}
    cur2=connections['default'].cursor()

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    oldtags=dictfetchall(cur2)
    context['oldtags']=oldtags

    if request.method == "POST":
        search_tag=0
        if 'selected_label' in request.POST and (request.POST['selected_label'] != '选择标签'):
            search_tag = request.POST['selected_label']
        else:
            for tag in oldtags:
                if tag['tag_name'] in request.POST:
                    search_tag = tag['tag_name']
        if search_tag==0:
            if 'cancelpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=0 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['cancelpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            elif 'setpublic' in request.POST:
                sql='update answer a,quesans qa set a.public=1 where qa.qno={0} and qa.ano=a.ano'
                cur2.execute(sql.format(request.POST['setpublic']))
                cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
                res=dictfetchall(cur2)
            else:
                #不是标签，要搜索内容
                key = "%"+request.POST['search']+"%"
                sql='select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                    from question q,quesans qa,answer a where (q.title like "{0}" or q.content like "{1}")\
                    and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(key,key)
                cur2.execute(sql)
                res=dictfetchall(cur2)
        else:
            #是标签，搜索标签
            sql1 = "select Tno from tags where tag_name = '{0}'".format(search_tag)
            cur2.execute(sql1)
            mid=cur2.fetchall()
            my_Tno = mid[0][0]

            sql2 = "select Qno from questags where Tno = {0}".format(my_Tno)
            cur2.execute(sql2)
            result=cur2.fetchall()
            res=[]

            for i in result:
                sql3 = 'select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                from question q,quesans qa,answer a where q.qno = {0}\
                and qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC'.format(i[0])
                cur2.execute(sql3)
                temp = dictfetchall(cur2)
                res.extend(temp)

    else:
        cur2.execute('select q.qno,q.title,q.time time1,a.ansname,a.time time2\
                     from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and a.public=1 and q.parent is NULL order by q.time DESC')
        res=dictfetchall(cur2)

    for i in range(len(res)):
        sql1 = "select Tno from questags where Qno = {0}".format(res[i]['qno'])
        cur2.execute(sql1)
        tagnums=cur2.fetchall()
    
        tags = []
        for tagnum in tagnums:
            sql2 = "select tag_name from tags where Tno = {0}".format(tagnum[0])
            cur2.execute(sql2)
            content = cur2.fetchall()
            tags.append(content[0][0])
        res[i]['tags']=tags

    context['publicques']=res

    cur2.execute('select qno,title,askno,askname,time,parent from question order by time DESC')
    ans=cur2.fetchall()
    result=[]
    for i in range(len(ans)):
        result.append(list(ans[i]))
    
    for i in range(len(result)):
        cur2.execute('select * from quesans where qno={0}'.format(result[i][0]))
        res=cur2.fetchall()
        if len(res)==0:
            result[i].append(0)
        else:
            result[i].append(1)

        sql='select public from answer a,quesans qa where qa.qno={0} and qa.ano=a.ano'
        cur2.execute(sql.format(int(result[i][0])))
        res=dictfetchall(cur2)
        if len(res)==0:
            result[i].append(0)
        elif result[i][5]==None and res[0]['public']==0:
            result[i].append(1)
        else:
            result[i].append(0)

    context['result']=result
    
    return render(request,'z_manager/manager-qa-1.html',context)

def manager_qa2(request,qid):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}
    cur2=connections['default'].cursor()
    
    if request.method == "POST":
        if 'titlechange' in request.POST:
            changemethod = 1
            changeid = request.POST['titlechange']
        elif 'contentchange' in request.POST:
            changemethod = 2
            changeid = request.POST['contentchange']
        else:
            changemethod = 3
            changeid = request.POST['answerchange']

        changecontent = request.POST['answer-content']
        QAchange(changemethod,changeid,changecontent)

    all_question = show_question(qid)
    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question
    context['lastqno']=question[-1]['qno']

    context['ifanswer']=1
    answer=[]
    cout=0
    for i in all_question:
        cur2.execute('select a.ano,a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
            if(cout == len(all_question)-1):
                context['ifanswer']=0
        cout=cout+1
    context['answer']=answer

    return render(request,'z_manager/manager-qa-2.html',context)

def manager_qa3(request,qid):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}
    context['ischild']=0
    cur2=connections['default'].cursor()

    all_question = show_question(qid)
    if(len(all_question)>1):
        context['ischild']=1

    sql = "select tag_name from tags order by citations DESC"
    cur2.execute(sql)
    result=dictfetchall(cur2)
    context['oldtags']=result
    
    if request.method == "POST":
        path=upload_file(request)
        if 'if-public' in request.POST:
            public=1
        else:
            public=0

        for tag in result:
            if tag['tag_name'] in request.POST:
                add_tags(tag['tag_name'],qid)
        if 'newtag' in request.POST and request.POST['newtag'] != "":
            tags = request.POST['newtag'].split("#")
            for tag in tags:
                add_tags(tag,qid)

        ans_question(request.session['user_id'],request.session['user_name'],request.POST['answer-content'],path,public,qid)
        return redirect('manager_qa4')

    question=[]
    for i in all_question:
        sql='select qno,title,content,askno,pic,time from question where qno={0}'
        cur2.execute(sql.format(i))
        result=dictfetchall(cur2)
        if(result[0]['pic']!=None):
            photo1 = result[0]['pic']
            pic = base64.b64encode(photo1)
            pic = bytes.decode(pic)
            result[0]['pic']=pic
        question.append(result[0])
    context['question']=question

    answer=[]
    for i in all_question:
        cur2.execute('select a.content,a.ansname,a.pic,a.time,q.qno from question q,quesans qa,answer a where qa.qno=q.qno and qa.ano=a.ano and q.qno={0}'.format(i))
        res=dictfetchall(cur2)
        if(len(res)!=0):
            if(res[0]['pic']!=None):
                photo2 = res[0]['pic']
                pic = base64.b64encode(photo2)
                pic = bytes.decode(pic)
                res[0]['pic'] = pic
            answer.append(res[0])
    context['answer']=answer

    return render(request,'z_manager/manager-qa-3.html',context)

def manager_qa4(request):
    power = request.session['user_power']
    if(power != '教秘'):
        return HttpResponse('页面打开错误！')
    
    context={}

    cur2=connections['default'].cursor()
    cur2.execute('select qno,title,askno,askname,time,parent from question where qno not in (select qno from quesans) order by time DESC')
    ans=dictfetchall(cur2)
    context['result']=ans

    return render(request,'z_manager/manager-qa-4.html',context)