# Create your views here.

from django.http import HttpResponse
from django.template.loader import get_template
from django.template import Context
from django.shortcuts import render
from django.http import Http404
from models import Report

def showReport(request):
    report_list = Report.objects.all()
    ctx = {}
    ctx['reports']=report_list
    print report_list[0].name
    return render(request, 'emergency.html',ctx)
    #return HttpResponse("Hello, world!")

def report(request):
    if request.method == 'POST':
        errors = []
        try:
            patientId = int(request.POST.get('patientId'))
            name = request.POST.get('name')
            latitude = float(request.POST.get('latitude'))
            longitude = float(request.POST.get('longitude'))
            dt = request.POST.get('datetime')
            datetime = datetime.strptime(dt,"%Y-%m-%d %H:%M:%S")
            incomingReport = Report(patientId=patientId, name=name, \
                    latitude=latitude, longitude=longitude, datetime=datetime)
            incomingReport.save()
            return HttpResponse("Report successfully added to database")
        except ValueError:
            return HttpResponse("Report not added properly")
    else:
        raise Http404
