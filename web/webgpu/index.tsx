import * as react from "react";

const shaders = `
struct VertexOut {
  @builtin(position) position : vec4f,
  @location(0) color : vec4f
}

@vertex
fn vertex_main(@location(0) position: vec4f,
               @location(1) color: vec4f) -> VertexOut
{
  var output : VertexOut;
  output.position = position;
  output.color = color;
  return output;
}

@fragment
fn fragment_main(fragData: VertexOut) -> @location(0) vec4f
{
  return fragData.color;
}
`;

const vertices = new Float32Array([
  0.0, 0.6, 0, 1, 1, 0, 0, 1, -0.5, -0.6, 0, 1, 0, 1, 0, 1, 0.5, -0.6, 0, 1, 0,
  0, 1, 1,
]);

let adapter;
let device;
let context;
let vertex_buffer;
let pipeline_descriptor;
let render_pipeline;

interface Navigator {
  gpu: any;
}

async function init() {
  if (!navigator.gpu) {
    throw Error("WebGPU not supported.");
  }

  adapter = await navigator.gpu.requestAdapter();
  if (!adapter) {
    throw Error("Couldn't request WebGPU adapter.");
  }

  device = await adapter.requestDevice();

  //let sampler = device.createSampler({
  //  maxAnisotropy: 0, // Invalid, maxAnisotropy must be at least 1.
  //});

  //device.popErrorScope().then((error) => {
  //  if (error) {
  //    // There was an error creating the sampler, so discard it.
  //    sampler = null;
  //    console.error(
  //      `An error occured while creating sampler: ${error.message}`
  //    );
  //  }
  //});

  const shader_module = device.createShaderModule({
    code: shaders,
  });

  const root = document.getElementById("root");
  const devicePixelRatio = window.devicePixelRatio || 1;
  canvas.width = canvas.clientWidth * devicePixelRatio * 1.5;
  canvas.height = canvas.clientHeight * devicePixelRatio * 1.5;

  context = canvas.getContext("webgpu");

  context.configure({
    device: device,
    format: navigator.gpu.getPreferredCanvasFormat(),
    alphaMode: "premultiplied",
  });

  // create buffer
  vertex_buffer = device.createBuffer({
    size: vertices.byteLength,
    usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
    //mappedAtCreation: true,
  });

  // transfer to gpu memory
  //new Float32Array(vertex_buffer.getMappedRange()).set(vertices);
  console.log(vertex_buffer);
  //vertex_buffer.unmap();

  device.queue.writeBuffer(vertex_buffer, 0, vertices, vertices.length);

  // attribute
  const vertex_buffers = [
    {
      attributes: [
        // location
        {
          shaderLocation: 0,
          offset: 0,
          format: "float32x4",
        },
        // color
        {
          shaderLocation: 1,
          offset: 4 * 4,
          format: "float32x4",
        },
      ],
      arrayStride: 4 * 8,
      stepMode: "vertex",
    },
  ];

  pipeline_descriptor = {
    vertex: {
      module: shader_module,
      entryPoint: "vertex_main",
      buffers: vertex_buffers,
    },
    fragment: {
      module: shader_module,
      entryPoint: "fragment_main",
      targets: [
        {
          format: navigator.gpu.getPreferredCanvasFormat(),
        },
      ],
    },
    primitive: {
      topology: "triangle-list",
    },
    layout: "auto",
  };
  render_pipeline = device.createRenderPipeline(pipeline_descriptor);
}

function onError(error) {
  //console.log(`Something bad happened: ${error.message}`);
  console.log(error);
  //device.destroy();
}
const validation = async () => {
  console.log("a validdation");
  // An example of poorly-structured error handling.
  device.pushErrorScope("out-of-memory");
  device.pushErrorScope("validation");
  device.pushErrorScope("internal");

  device.popErrorScope().then(onError);
  device.popErrorScope().then(onError);
  device.popErrorScope().then(onError);
};

const render = async () => {
  const commandEncoder = device.createCommandEncoder();

  const clearColor = { r: 0.0, g: 0.0, b: 0.0, a: 1 };
  const view = context.getCurrentTexture().createView();

  const renderPassDescriptor = {
    colorAttachments: [
      {
        clearValue: clearColor,
        loadOp: "clear",
        storeOp: "store",
        view: view,
      },
    ],
  };

  // begin
  const passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);

  passEncoder.setPipeline(render_pipeline);
  passEncoder.setVertexBuffer(0, vertex_buffer);
  passEncoder.draw(3, 1, 0, 0);

  // end
  passEncoder.end();
  device.queue.submit([commandEncoder.finish()]);

  requestAnimationFrame(render);
};

const main = async () => {
  await init();
  setInterval(validation, 1000);
  requestAnimationFrame(render);
};

main()
  .then(() => {
    console.log("running..");
  })
  .catch((err) => console.log(err));
