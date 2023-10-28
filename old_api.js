import express from "express";
import sqlite3 from "sqlite3";

const db = new sqlite3.Database("./finger.sqlite3");
const app = express();
app.use(express.json());
const port = 5000;

db.serialize(() => {
    db.run("drop table if exists finger");
    db.run("Create table if not exists finger (id integer primary key, finger integer, component text)");
    db.run("Insert into finger (finger, component) values (1, 'Scissor');");
});

app.use((req, res, next) => {
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    res.header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
    next();
});

app.get("/api/hand_gesture/", (req, res) => {
    db.all("Select finger from finger;", [],  (err, rows) => {
        res.json(rows[rows.length-1]);
        console.log(rows[rows.length - 1]);
        // status: !err ? "ok" : "error", 
    });
});

app.post("/api/hand_gesture/", (req, res) => {
    db.each("Select component from finger ORDER BY id DESC LIMIT 1", [], (err, rows) => {
        if (rows.component != req.body.component) {
            db.run("Insert into finger (finger, component) values (?, ?);", 
                [req.body.finger, req.body.component], (err) => {
                res.json({status: !err ? "inserted" : "error"});
                if (err) {
                    console.log(err);
                }
            });
        }
        else {
            res.json({status: "error"});
        }
    });
});

app.listen(port, "192.168.117.66", () => {
    console.log(`api listening at ${port}`);
});

