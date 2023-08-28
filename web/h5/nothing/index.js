const panic = (msg) => { alert(msg); throw new Error(msg); }


// initialize
const canvas = document.querySelector("canvas");
if (!navigator.gpu) {
    panic("WebGPU not supported on this browser.");
}
const adapter = await navigator.gpu.requestAdapter();
if (!adapter) {
    panic("request adapter failed!");
}
const device = await adapter.requestDevice();
const context = canvas.getContext("webgpu")
const canvasFormat = navigator.gpu.getPreferredCanvasFormat();
context.configure({
    device: device,
    format: canvasFormat,
})


const [cellPipeline, cellVertexBuffer, cellVertices] = (() => {
    const vertices = new Float32Array([
        //   X,    Y,
        -0.8, -0.8, // Triangle 1 (Blue)
        0.8, -0.8,
        0.8, 0.8,

        -0.8, -0.8, // Triangle 2 (Red)
        0.8, 0.8,
        -0.8, 0.8,
    ]);

    const vertexBuffer = device.createBuffer({
        label: "Cell vertices",
        size: vertices.byteLength,
        usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
    })

    device.queue.writeBuffer(vertexBuffer, 0, vertices);

    const vertexBufferLayout = {
        arrayStride: 8,
        attributes: [{
            format: "float32x2",
            offset: 0,
            shaderLocation: 0,
        }]
    }

    const cellShaderModule = device.createShaderModule({
        label: "Cell shader",
        code: `
            @vertex
            fn vertexMain(
                @location(0) pos: vec2f
            ) -> @builtin(position) vec4f {
                return vec4f(pos, 0, 1);
            }

            @fragment
            fn fragmentMain() -> @location(0) vec4f{
                //let red = rand();
                return vec4f(0.4, 0.8,  0 , 1);
            }
        `
    });

    const pipeline = device.createRenderPipeline({
        label: "Cell pipelinie",
        layout: "auto",
        vertex: {
            module: cellShaderModule,
            entryPoint: "vertexMain",
            buffers: [vertexBufferLayout]
        },
        fragment: {
            module: cellShaderModule,
            entryPoint: "fragmentMain",
            targets: [{
                format: canvasFormat
            }]
        },
    })

    return [pipeline, vertexBuffer, vertices]
})();



const rende = (dt) => {

    const encoder = device.createCommandEncoder();

    const pass = encoder.beginRenderPass({
        colorAttachments: [
            {
                view: context.getCurrentTexture().createView(),
                loadOp: "clear",
                clearValue: [0, 0, 0.4, 1],
                storeOp: "store",
            }]
    });

    pass.setPipeline(cellPipeline);
    pass.setVertexBuffer(0, cellVertexBuffer)
    pass.draw(cellVertices.length / 2);

    pass.end(); // finish before create commandbuffer


    //const commandBuffer = encoder.finish();
    //device.queue.submit([commandBuffer]);
    device.queue.submit([encoder.finish()]);
};


rende(0);

