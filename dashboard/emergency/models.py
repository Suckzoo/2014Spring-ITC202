from django.db import models
from django.contrib import admin
#from emergency.models import blahblah
# Create your models here.
class Report(models.Model):
     patientId = models.IntegerField(default=0)
     name = models.CharField(max_length=50)
     latitude = models.FloatField(default=0)
     longitude = models.FloatField(default=0)
     datetime = models.DateTimeField(default=0)

admin.site.register(Report)
