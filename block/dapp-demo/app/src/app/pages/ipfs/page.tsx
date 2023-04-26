
'use client'

import Image from "next/image"
import { useState } from "react"

import { Web3Storage, getFilesFromPath } from 'web3.storage'


const API = process.env.web3_storage_api
const storage = new Web3Storage({ token: "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJkaWQ6ZXRocjoweDNhMTk0N0QzMEIwNzI2OTkyNjFDMjFkMWE1ZTkzZEQ4RTJjYzRiZEQiLCJpc3MiOiJ3ZWIzLXN0b3JhZ2UiLCJpYXQiOjE2ODI0NDU3NDA4MTgsIm5hbWUiOiJkYXBwLWRlbW8ifQ.EyD_u75hTAMhzLoRdUwh2gJL1VeNr9rWddYhxdAa9ug" })

export default function IPFS() {


    const [file, setFile] = useState()
    const [urlArry, setUrlArry] = useState([])


    const retrieveFile = (e: any) => {
        const data = e.target.files[0];

        setFile(data)

        e.preventDefault();
    }

    const handleSubmit = async (e: any) => {
        e.preventDefault();
        if (!file) {
            console.log("file is null");
            return;
        }

        try {
            const CID = await storage.put([file])
            console.log(`CID: ${CID}`)
            const url = `https://ipfs.infura.io/ipfs/${CID}/${(file as File).name}`;
            setUrlArry((prev) => [...prev, url] as any)
        } catch (error: any) {
            console.log(error.message);
        }
    }


    return (
        <div className="IPFS">
            <header className="IPFS-header">IPFS prj</header>

            <div className="main">
                <form onSubmit={handleSubmit}>
                    <input type={"file"} onChange={retrieveFile} />
                    <button type="submit" className="button"> Submit</button>
                </form>

                <div className="display">
                    {
                        urlArry.length !== 0 ? urlArry.map(
                            (el, index) => {
                                return <img src={el} width="200" height="200" key={index}></img>
                            }) : <h3>Upload file</h3>
                    }
                </div>

            </div>
        </div >
    )
}
