from calendar import c
from flask import Flask, request, Response
import string
import random
import json
from flask_sqlalchemy import SQLAlchemy
from werkzeug.utils import secure_filename
from sqlalchemy.sql.expression import select
app = Flask(__name__)

# app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://postgres:root@localhost:5432/postgres'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///sq.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)


class SuitCase(db.Model):
    # id
    id = db.Column(db.Integer, primary_key=True)
    uid = db.Column(db.String(16), unique=True, nullable=False)

    # data
    weight = db.Column(db.Float)
    status = db.Column(db.Integer, nullable=False)
    surname = db.Column(db.Text, nullable=False)
    name = db.Column(db.Text, nullable=False)
    patronymics = db.Column(db.Text, nullable=False)
    flight = db.Column(db.Text, nullable=False)
    city = db.Column(db.Text, nullable=False)

    # img
    first_img = db.Column(db.TEXT,  nullable=False)
    second_img = db.Column(db.Text, nullable=False)
    third_img = db.Column(db.Text, nullable=False)
    fourth_img = db.Column(db.Text,  nullable=False)
    face_img = db.Column(db.Text,  nullable=False)

    first_img_name = db.Column(db.TEXT, nullable=False)
    second_img_name = db.Column(db.Text, nullable=False)
    third_img_name = db.Column(db.Text, nullable=False)
    fourth_img_name = db.Column(db.Text, nullable=False)
    face_img_name = db.Column(db.Text,  nullable=False)

    first_img_mimetype = db.Column(db.TEXT, nullable=False)
    second_img_mimetype = db.Column(db.Text, nullable=False)
    third_img_mimetype = db.Column(db.Text, nullable=False)
    fourth_img_mimetype = db.Column(db.Text, nullable=False)
    face_img_mimetype = db.Column(db.Text,  nullable=False)

    def __repr__(self) -> str:
        return f"{self.uid}"


db.create_all()


@app.route('/')
def hello_world():
    return 'Hello, World!'


@app.route('/upload', methods=['POST'])
def upload():
    print(request.files)

    name = request.form["name"]
    surname = request.form["surname"]
    patronymics = request.form["patronymics"]
    weight = request.form["weight"]
    flight = request.form["flight"]
    city = request.form["city"]

    pic1 = request.files['pic1']
    pic2 = request.files['pic2']
    pic3 = request.files['pic3']
    pic4 = request.files['pic4']
    face = request.files['face']

    if not pic1:
        return 'No pic1 uploaded!', 400
    if not pic2:
        return 'No pic2 uploaded!', 400
    if not pic3:
        return 'No pic3 uploaded!', 400
    if not pic4:
        return 'No pic4 uploaded!', 400
    if not face:
        return 'No face uploaded!', 400

    filename1 = secure_filename(pic1.filename)
    mimetype1 = pic1.mimetype

    filename2 = secure_filename(pic2.filename)
    mimetype2 = pic2.mimetype

    filename3 = secure_filename(pic3.filename)
    mimetype3 = pic3.mimetype

    filename4 = secure_filename(pic4.filename)
    mimetype4 = pic4.mimetype

    filenameface = secure_filename(face.filename)
    mimetypeface = face.mimetype

    if not filename1 or not mimetype1:
        return 'Bad upload!', 400
    if not filename2 or not mimetype2:
        return 'Bad upload!', 400
    if not filename3 or not mimetype3:
        return 'Bad upload!', 400
    if not filename4 or not mimetype4:
        return 'Bad upload!', 400
    if not filenameface or not mimetypeface:
        return 'Bad upload!', 400

    rrandom = ''.join([random.choice(string.ascii_letters
                                     + string.digits) for n in range(16)])

    img = SuitCase(first_img=pic1.read(), second_img=pic2.read(), third_img=pic3.read(), fourth_img=pic4.read(), face_img=face.read(), first_img_name=filename1, face_img_name=filenameface, first_img_mimetype=mimetype1, second_img_name=filename2,
                   second_img_mimetype=mimetype2, third_img_name=filename3, third_img_mimetype=mimetype3, fourth_img_name=filename4, fourth_img_mimetype=mimetype4, face_img_mimetype=mimetypeface, status=1, surname=surname, name=name, flight=flight, uid=rrandom, patronymics=patronymics, weight=weight, city=city)
    db.session.add(img)
    db.session.commit()
    print(rrandom)

    return rrandom, 200


@app.route('/status/<id>/<status>')
def status(id, status):
    m = SuitCase.query.all()
    print(m)
    for v in m:
        print(v)
        if str(v) == str(id):
            db.session.execute(select(SuitCase).where(
                SuitCase.uid == id)).scalar_one().status = status
            db.session.flush()
            db.session.commit()
    return "write is done"

@app.route('/status/<id>/<status>')
def city(id, city):
    m = SuitCase.query.all()
    print(m)
    for v in m:
        print(v)
        if str(v) == str(id):
            db.session.execute(select(SuitCase).where(
                SuitCase.uid == id)).scalar_one().city = city
            db.session.flush()
            db.session.commit()
    return "write is done"

@app.route("/getphoto/<int:num>/<id>")
def getphoto(num, id):

    data = db.session.execute(select(SuitCase).where(
        SuitCase.uid == id)).scalar_one()
    db.session.flush()
    db.session.commit()

    match int(num):
        case 1: return Response(data.first_img, mimetype=data.first_img_mimetype)
        case 2: return Response(data.second_img, mimetype=data.second_img_mimetype)
        case 3: return Response(data.third_img, mimetype=data.third_img_mimetype)
        case 4: return Response(data.fourth_img, mimetype=data.fourth_img_mimetype)
        case 5: return Response(data.face_img, mimetype=data.face_img_mimetype)


@app.route("/getdata/<id>")
def getdata(id):

    data = db.session.execute(select(SuitCase).where(
        SuitCase.uid == id)).scalar_one()
    data_json = {
        "name": data.name,
        "surname": data.surname,
        "patronymics": data.patronymics,
        "weight": data.weight,
        "flight": data.flight,
        "status": data.status,
        "city": data.city
    }
    db.session.flush()
    db.session.commit()

    return json.dumps(data_json)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
