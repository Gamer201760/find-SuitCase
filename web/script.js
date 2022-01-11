function myFunction(){
    
    let name = document.getElementById("name").value;
    let suraname = document.getElementById("surname").value;
    let flight = document.getElementById("flight").value;
    let pat = document.getElementById("patronymics").value;
    let weight = document.getElementById("weight").value;
    console.log(name, suraname, flight, pat, weight);

    eel.send(name,suraname,flight,pat,weight);
}
function mekaphoto(){eel.make_photo();}

eel.expose(statuscode);
function statuscode(data) {alert(data)}