function myFunction(){
    let name = document.getElementById("name").value;
    let fname = document.getElementById("fname").value;
    let flight = document.getElementById("flight").value;

    eel.send(name,fname,flight);
}
function mekaphoto(){
    eel.make_photo();
}
eel.expose(statuscode);
function statuscode(data) {
    alert(data)
}