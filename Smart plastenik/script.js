import { initializeApp } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js";
import { 
    getDatabase,
    ref,
    child,
    update,
    set,
    get,
} from "https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js";
  const firebaseConfig = {
    apiKey: "AIzaSyCn75QqUymZshV0Ewg1qRgPJBJWbU7Qfro",
    authDomain: "iot-plastenik.firebaseapp.com",
    databaseURL: "https://iot-plastenik-default-rtdb.europe-west1.firebasedatabase.app/",
    projectId: "iot-plastenik",
    storageBucket: "iot-plastenik.appspot.com",
    messagingSenderId: "150960520232",
    appId: "1:150960520232:web:900d0b7df0a89acadb491d"
  };

const app = initializeApp(firebaseConfig);
const db = getDatabase();


const temperaturaDb = ref(db, 'Senzor/temperatura');
const vlaznostDb = ref(db,'Senzor/vlaznost');
  setInterval(updateMQ2Value, 1000);
function updateMQ2Value() {
    get(temperaturaDb).then((snapshot) => {
      if (snapshot.exists()) {
        const temperatura = snapshot.val();
        document.getElementById('temperatura').value = temperatura;
        if(temperatura > 25){
          document.querySelector('.upozorenje').style.display = 'block';
        }
        else{
          document.querySelector('.upozorenje').style.display = 'none';
        }
      } else {
        console.log('Vrijednost MQ2Vrijednosti nije pronađena.');
      }
    }).catch((error) => {
      console.error('Došlo je do greške pri čitanju MQ2Vrijednosti:', error);
    });
    get(vlaznostDb).then((snapshot) =>{
      if(snapshot.exists()){
        const vlaznost = snapshot.val();
        document.getElementById('vlaznost').value = vlaznost;
      }
    })
}