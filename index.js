const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const PORT = 3000; // You can use any available port

// Middleware to parse JSON payloads
app.use(bodyParser.json());

// POST endpoint to receive coordinates
app.post("/api/coordinates", (req, res) => {
    const { latitude, longitude } = req.body;

    // Log the coordinates (replace this with additional functionality if needed)
    console.log("Received Coordinates:");
    console.log("Latitude:", latitude);
    console.log("Longitude:", longitude);

    // Respond to ESP32 with a success message
    res.status(200).json({ message: "Coordinates received successfully!" });
});

// Start the server
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
