"""ranksystem URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path,re_path
from . import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('index/', views.index,name='index'),
    path('logout/',views.log_out,name='logout'),
    path('upload_file/',views.upload_file,name='upload_file'),
    path('vruclogin/',views.vruc_login,name='vruc_login'),
    path('cas/',views.cas,name='cas'),

    path('manager/',views.manager,name='manager'),
    path('manager/manager-check-1', views.manager_check1, name='manager_check1'),
    path('manager/manager-check-2', views.manager_check2, name='manager_check2'),
    path('manager/manager-check-3', views.manager_check3, name='manager_check3'),
    path('manager/manager-input-1', views.manager_input1, name='manager_input1'),
    path('manager/manager-input-2', views.manager_input2, name='manager_input2'),
    path('manager/manager-input-3', views.manager_input3, name='manager_input3'),
    path('manager/manager-input-4', views.manager_input4, name='manager_input4'),
    path('manager/manager-input-5', views.manager_input5, name='manager_input5'),
    path('manager/manager-qa-1', views.manager_qa1, name='manager_qa1'),
    re_path(r'manager/manager-qa-2(?P<qid>\d+)/', views.manager_qa2, name='manager_qa2'),
    re_path(r'manager/manager-qa-3(?P<qid>\d+)/', views.manager_qa3, name='manager_qa3'),
    path('manager/manager-qa-4', views.manager_qa4, name='manager_qa4'),

    path('student/',views.student,name='student'),
    path('student/student-check-1', views.student_check1, name='student_check1'),
    path('student/student-check-2', views.student_check2, name='student_check2'),
    path('student/student-check-3', views.student_check3, name='student_check3'),
    path('student/student-qa-1', views.student_qa1, name='student_qa1'),
    re_path(r'student/student-qa-2(?P<qid>\d+)/', views.student_qa2, name='student_qa2'),
    path('student/student-qa-3', views.student_qa3, name='student_qa3'),
    re_path(r'student/student-qa-3(?P<qid>\d+)/', views.student_qa3plus, name='student_qa3plus'),
    path('student/student-qa-4', views.student_qa4, name='student_qa4'),

    path('headteacher/headteacher-check-1', views.headteacher_check1, name='headteacher_check1'),
    path('headteacher/headteacher-check-2', views.headteacher_check2, name='headteacher_check2'),

    path('institution/',views.institution,name='institution'),
    path('institution/institution-check-1', views.institution_check1, name='institution_check1'),
    path('institution/institution-check-2', views.institution_check2, name='institution_check2'),
    path('institution/institution-check-3', views.institution_check3, name='institution_check3'),
    path('institution/institution-qa-1', views.institution_qa1, name='institution_qa1'),
    re_path(r'institution/institution-qa-2(?P<qid>\d+)/', views.institution_qa2, name='institution_qa2'),
    re_path(r'institution/institution-qa-3(?P<qid>\d+)/', views.institution_qa3, name='institution_qa3'),
    path('institution/institution-qa-4', views.institution_qa4, name='institution_qa4'),

    path('qateacher/qateacher-qa-1', views.qateacher_qa1, name='qateacher_qa1'),
    re_path(r'qateacher/qateacher-qa-2(?P<qid>\d+)/', views.qateacher_qa2, name='qateacher_qa2'),
    re_path(r'qateacher/qateacher-qa-3(?P<qid>\d+)/', views.qateacher_qa3, name='qateacher_qa3'),
    path('qateacher/qateacher-qa-4', views.qateacher_qa4, name='qateacher_qa4'),
]