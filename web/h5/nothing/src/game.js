import { CellRender } from "./cell_render.js";

const panic = (msg) => { alert(msg); throw new Error(msg); }

export class WebGPU {
    device;
    context;
    canvasFormat;

    async init(canvas) {
        if (!navigator.gpu) {
            panic("WebGPU not supported on this browser.");
        }
        const adapter = await navigator.gpu.requestAdapter();
        if (!adapter) {
            panic("request adapter failed!");
        }

        this.device = await adapter.requestDevice();
        this.context = canvas.getContext("webgpu")
        this.canvasFormat = navigator.gpu.getPreferredCanvasFormat();

        this.context.configure({
            device: this.device,
            format: this.canvasFormat,
        })
    }
}

const canvas = document.querySelector("canvas");
let gpu = new WebGPU()
await gpu.init(canvas)
console.log(gpu.device)



export class Game {

    constructor() {
        this.cellRender = CellRender(gpu.device, gpu.canvasFormat)
    }

    init() {

    }

    run() {
        this.rende(0)
    }

    rende = (dt) => {
        const encoder = gpu.device.createCommandEncoder();

        const pass = encoder.beginRenderPass({
            colorAttachments: [
                {
                    view: gpu.context.getCurrentTexture().createView(),
                    loadOp: "clear",
                    clearValue: [0, 0, 0.4, 1],
                    storeOp: "store",
                }]
        });

        pass.setPipeline(this.cellRender.Pipeline);
        pass.setVertexBuffer(0, this.cellRender.VertexBuffer)

        pass.setBindGroup(0, this.cellRender.BindGroup)

        pass.draw(this.cellRender.Vertices.length / 2, this.cellRender.GridSize * this.cellRender.GridSize);

        pass.end(); // finish before create commandbuffer


        //const commandBuffer = encoder.finish();
        //device.queue.submit([commandBuffer]);
        gpu.device.queue.submit([encoder.finish()]);
    };

}

