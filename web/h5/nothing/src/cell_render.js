
export const CellRender = (device, canvasFormat) => {

    const GRID_SIZE = 16;

    // vertex
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
        label: "Cell Vertices",
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

    // cell state
    const cellStateArrray = new Uint32Array(GRID_SIZE * GRID_SIZE);
    const cellStateStorage = device.createBuffer({
        label: "Cell State",
        size: cellStateArrray.byteLength,
        usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    })
    for (let i = 0; i < cellStateArrray.length; i += 3) {
        cellStateArrray[i] = 1;
    }
    device.queue.writeBuffer(cellStateStorage, 0, cellStateArrray)



    // shader
    const cellShaderModule = device.createShaderModule({
        label: "Cell shader",
        code: `
            @group(0) @binding(0) var<uniform> grid: vec2f;
            @group(0) @binding(1) var<storage> cellState: array<u32>;

            //----------------------

            struct VertexInput{
                @location(0) pos: vec2f,
                @builtin(instance_index) instance: u32,
            };

            struct VertexOutput{
                @builtin(position) pos: vec4f,
                @location(0) cell: vec2f,
            }


            @vertex
            fn vertexMain(input: VertexInput)  -> VertexOutput {

                let i = f32(input.instance);

                let cell = vec2f( i % grid.x, floor(i/ grid.y));
                let state =f32(cellState[input.instance]);

                let cellOffset = cell/ grid * 2;
                let gridPos = (input.pos * state + 1) /grid -1 + cellOffset;

                var o : VertexOutput;
                o.pos = vec4f(gridPos, 0, 1);
                o.cell = cell;
                return o;

            }

            //---------------------------------

            @fragment
            fn fragmentMain(input: VertexOutput) -> @location(0) vec4f{
                let c = input.cell/ grid;
                return vec4f(c, 1-c.x, 1);
            }
        `
    });

    // pipeline
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

    // uniform
    const uniformArray = new Float32Array([GRID_SIZE, GRID_SIZE])
    const uniformBuffer = device.createBuffer({
        label: "Grid Unifroms",
        size: uniformArray.byteLength,
        usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST
    });
    device.queue.writeBuffer(uniformBuffer, 0, uniformArray);


    // binding
    const bindGroup = device.createBindGroup({
        label: "Cell renderer bind group",
        layout: pipeline.getBindGroupLayout(0),
        entries: [
            {
                binding: 0,
                resource: { buffer: uniformBuffer }
            },
            {
                binding: 1,
                resource: { buffer: cellStateStorage }
            }
        ],
    });


    return {
        Pipeline: pipeline,
        Vertices: vertices,
        VertexBuffer: vertexBuffer,
        BindGroup: bindGroup,
        GridSize: GRID_SIZE
    }
};