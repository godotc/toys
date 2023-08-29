
export const CellRender = (device, canvasFormat) => {
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
            struct VertexInput{
                @location(0) pos: vec2f,
                @builtin(instance_index) instance: u32,
            };

            struct VertexOutput{
                @builtin(position) pos: vec4f,
                @location(0)cell: vec2f,
            }

            @group(0) @binding(0) var<uniform> grid: vec2f;

            @vertex
            fn vertexMain(input: VertexInput)  -> VertexOutput {

                let i = f32(input.instance);

                let cell = vec2f( i % grid.x, floor(i/ grid.y));

                let cellOffset = cell/ grid * 2;
                let gridPos = (input.pos + 1)/ grid -1 + cellOffset;

                var o : VertexOutput;
                o.pos = vec4f(gridPos, 0, 1);
                o.cell = cell;
                return o;

            }

            @fragment
            fn fragmentMain() -> @location(0) vec4f{
                var i : f32 = 0;
                i+=1;
                let red = sin(i);
                return vec4f(red,  0.8,  0 , 1);
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

    const GRID_SIZE = 16;

    const bindGroup = (() => {
        const uniformArray = new Float32Array([GRID_SIZE, GRID_SIZE])
        const uniformBuffer = device.createBuffer({
            label: "Grid Unifroms",
            size: uniformArray.byteLength,
            usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST
        });
        device.queue.writeBuffer(uniformBuffer, 0, uniformArray);

        return device.createBindGroup({
            label: "Cell renderer bind group",
            layout: pipeline.getBindGroupLayout(0),
            entries: [{
                binding: 0,
                resource: { buffer: uniformBuffer }
            }],
        });
    })();


    return {
        Pipeline: pipeline,
        Vertices: vertices,
        VertexBuffer: vertexBuffer,
        BindGroup: bindGroup,
        GridSize: GRID_SIZE
    }
};