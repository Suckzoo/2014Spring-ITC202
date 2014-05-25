from django.db import models
from django.contrib import admin

class Profile(models.Model):
    patientId = models.IntegerField(default=0)
    name = models.CharField(max_length=50)
    birthday = models.DateField(default=0)
    isMale = models.BooleanField()
    history = models.TextField()

class Question(models.Model):
    patientId = models.IntegerField(default=0)
    doctorId = models.IntegerField(default=0)
    isDoctorToPatient = models.BooleanField()
    context = models.TextField()
