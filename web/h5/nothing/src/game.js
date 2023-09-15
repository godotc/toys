import { CellRender } from "./cell_render.js";

const panic = (msg) => { alert(msg); throw new Error(msg); }
const print = (msg) => console.log(msg)

class WebGPU {
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
const gpu = new WebGPU()
await gpu.init(canvas)
console.log(gpu.device)



export class Game {
    tickInterval = 200;
    step = 0;
    cellRender = undefined;
    bLoop = true;

    constructor(Config) {
        // NOTICE this!
        {
            this.loop = this.loop.bind(this);
            this.update = this.update.bind(this);
            this.render = this.render.bind(this);
        }

        this.bLoop = Config.bLoop;
        this.tickInterval = Config.tick_interval;

        this.cellRender = CellRender(gpu.device, gpu.canvasFormat)
        print(this.cellRender)
    }

    init() {

    }

    run() {
        print(this.cellRender)
        print(this.tickInterval)

        const interval = this.tickInterval;

        this.loop()
        if (this.bLoop)
            setInterval(this.loop, interval);
        else
            setTimeout(this.loop, interval);
    }

    loop() {
        const encoder = gpu.device.createCommandEncoder();
        {
            {
                const compute_pass = encoder.beginComputePass();

                this.cellRender.Compute(compute_pass, this.step);


                compute_pass.end();
            }

            ++this.step;

            {
                const render_pass = encoder.beginRenderPass({
                    colorAttachments: [
                        {
                            view: gpu.context.getCurrentTexture().createView(),
                            loadOp: "clear",
                            clearValue: [0, 0, 0.4, 1],
                            storeOp: "store",
                        }
                    ]
                });

                this.cellRender.Draw(render_pass, this.step)

                render_pass.end(); // finish before create commandbuffer
            }

        }
        //const commandBuffer = encoder.finish();
        //device.queue.submit([commandBuffer]);
        gpu.device.queue.submit([encoder.finish()]);

    }

    update = (dt) => {


    }

    render = (dt) => {

    };

}

