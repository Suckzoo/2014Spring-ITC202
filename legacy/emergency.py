from flask import Flask
from flask import render_template
from flask import request
from flask import redirect
import sqlite3 as lite

app = Flask(__name__)
token = '123456789'
@app.route("/")
def listing():
    return render_template('index.html')

@app.route("/data/<id>")
def showData(id):
    return render_template('analysis.html')

@app.route("/progress/<id>")
def setProgress(id):
    return render_template('progress.html')

@app.route("/alert",methods=['POST'])
def getInfo():
    if request.form['server_token']!=token:
        return redirect('/error')
        #return render_template('error.html')
    #dbConnection = lite.connect('emergency.db')
    #dbCursor = dbConnection.cursor()
    print "What to add?"

@app.errorhandler(404)
def notFoundException(error):
    return render_template('error/404.html')

@app.errorhandler(500)
def internalServerError(error):
    return render_template('error/500.html')

@app.errorhandler(405)
def methodNotAllowedException(error):
    return redirect('/error')

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=1190)
