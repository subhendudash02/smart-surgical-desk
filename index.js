import { createClient } from '@supabase/supabase-js';
import express from "express";

const supabase = createClient('https://ztvjlvcntofhedvagtdy.supabase.co', 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inp0dmpsdmNudG9maGVkdmFndGR5Iiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTgzOTcwNDEsImV4cCI6MjAxMzk3MzA0MX0.5f7VBafg8RotQ4Jm71Iy5mr8IUaVjzxeML72oHsaFj4');
const app = express();
app.use(express.json());
const port = 5000;

app.use((req, res, next) => {
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    res.header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
    next();
});

app.get("/api/hand_gesture/", async (req, res) => {
    const { data, error } = await supabase.from('component_log').select('*');
    res.send(data[0]);
});

app.post("/api/hand_gesture/", async (req, res) => {
    // Select component from component_log ORDER BY id DESC LIMIT 1
    const { data, error } = await supabase.from('component_log').select('component')
                            .order('id', { ascending: false }).limit(1);
    
    if (data[0].component != req.body.component) {
        // Insert new component to component_log
        const { data, error } = await supabase.from('component_log').insert([
            { component: req.body.component, finger: req.body.finger }
        ]);
        res.json({status: !error ? "inserted" : error});
    }
    else {
        res.json({status: "ok", msg: "prev data entered already"});
    }
});

app.listen(port, () => {
    console.log(`api listening at ${port}`);
});