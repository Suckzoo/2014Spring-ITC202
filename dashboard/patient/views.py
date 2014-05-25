# Create your views here.

from django.http import HttpResponse
from django.template.loader import get_template
from django.template import Context
from django.shortcuts import render
from django.http import Http404
#from models import 

def overview(request):
    return render(request, 'patient.html')

def question(report):
    return render(request, 'patient-question.html')
