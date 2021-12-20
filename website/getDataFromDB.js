const { MongoClient } = require('mongodb');

//Nastavi url, klienta in bazo
const url = 'mongodb+srv://anzedolenc:iiks2021@cluster0.6svfx.mongodb.net/tempDB?retryWrites=true&w=majority';
const client = new MongoClient(url);
const dbName = 'tempDB';

//Dobi podatke o temperaturi za zadnjih 31 dni po urah
async function main() {
    var seznamUre = [];
    var seznamTemp = [];
    var seznamDan = [];

    await client.connect();
    console.log('Connected successfully to server');
    const db = client.db(dbName);
    const collection = db.collection('temperatura');
    var seznam = [];
    var i = 0;
    var j = 0;
    for (dan = 0; dan < 31; dan++) {
        for (ura = 0; ura < 24; ura++) {
            const findResult = await collection.findOne({ 'day': dan, 'hour': ura }, { projection: { _id: 0, 'day':1, 'hour': 1, 'temperatura': 1} });
            if (findResult != null) {
                seznamUre[i] = (Object.values(findResult)[1]);
                seznamTemp[i] = (Object.values(findResult)[2]);
                
                i++;
            }
        }
        seznamDan[j] = (Object.values(findResult)[0]); 
        j++;
    }   
    return (seznamTemp, seznamUre, seznamDan);
}

let val = main()
    .then(console.log)
    .catch(console.error)
    .finally(() => client.close());

    /*
var temp = [21.3, 21.3, 21.7, 21.1, 22.0, 23.1, 22.5, 22.7, 22.5, 23.0, 23.5, 23.0, 22.5, 22.3, 22.0, 21.5, 22.4, 23.0, 22.3, 21.8, 21.8, 21.5, 22.0, 21.6];
var ure = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23];
var dan = [19];

console.log(temp);
console.log(ure)
console.log(dan)
*/