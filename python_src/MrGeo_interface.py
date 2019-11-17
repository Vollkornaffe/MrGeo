from ctypes import cdll, c_double, c_void_p, c_size_t, POINTER, c_int

class SurfaceData:
    def __init__(self, u_degree = 3, v_degree = 3, u_numControl = 4, v_numControl = 4, u_numSamples = 100, v_numSamples = 100):
        self.u_degree = u_degree
        self.v_degree = v_degree
        self.u_numControl = u_numControl
        self.v_numControl = v_numControl
        self.u_numSamples = u_numSamples
        self.v_numSamples = v_numSamples

def Clib(path):

    lib = cdll.LoadLibrary(path)

    lib.QF_construct.argtypes = [c_size_t]
    lib.QF_construct.restype = c_void_p

    lib.QF_torus_test.argtypes = [c_void_p]
    lib.QF_torus_test.restype = None

    lib.AD_construct.argtypes = []
    lib.AD_construct.restype = c_void_p

    lib.AD_test_bases.argtypes = [c_void_p, c_void_p, c_size_t, c_size_t]
    lib.AD_test_bases.restype = None

    lib.AD_fill.argtypes = [c_void_p, c_void_p, c_size_t, c_size_t]
    lib.AD_fill.restype = None

    lib.AD_get_num_objects.argtypes = [c_void_p]
    lib.AD_get_num_objects.restype = c_size_t

    lib.AD_get_num_frames.argtypes = [c_void_p]
    lib.AD_get_num_frames.restype = c_size_t

    lib.AD_get_scales.argtypes = [c_void_p, c_size_t]
    lib.AD_get_scales.restype = POINTER(c_double * 3)

    lib.AD_get_locations.argtypes = [c_void_p, c_size_t]
    lib.AD_get_locations.restype = POINTER(c_double * 3)

    lib.AD_get_rotations.argtypes = [c_void_p, c_size_t]
    lib.AD_get_rotations.restype = POINTER(c_double * 4)

    lib.CS_construct.argtypes = [c_size_t, c_size_t, c_size_t, c_size_t, c_size_t, c_size_t]
    lib.CS_construct.restype = c_void_p

    lib.CS_get_control.argtypes = [c_void_p, c_size_t]
    lib.CS_get_control.restype = POINTER(c_double)

    lib.CS_compute_samples.argtypes = [c_void_p]
    lib.CS_compute_samples.restype = None

    lib.CS_get_samples.argtypes = [c_void_p, c_size_t]
    lib.CS_get_samples.restype = POINTER(c_double)

    def init(self, surface_data, max_spin):
        self.qf_obj = lib.QF_construct(max_spin)
        self.ad_obj = lib.AD_construct()
        self.cs_obj = lib.CS_construct(surface_data.u_degree, surface_data.v_degree, surface_data.u_numControl, surface_data.v_numControl, surface_data.u_numSamples, surface_data.v_numSamples)

    def torus_test(self):
        lib.QF_torus_test(self.qf_obj)

    def test_bases(self, num_lines = 1, num_frames = 1000):
        lib.AD_test_bases(self.ad_obj, self.qf_obj, num_lines, num_frames)

    def fill(self, num_lines = 1, num_frames = 1000):
        lib.AD_fill(self.ad_obj, self.qf_obj, num_lines, num_frames)

    def get_num_objects(self):
        return lib.AD_get_num_objects(self.ad_obj)

    def get_num_frames(self):
        return lib.AD_get_num_frames(self.ad_obj)

    def get_scales(self, i):
        return lib.AD_get_scales(self.ad_obj, i)

    def get_locations(self, i):
        return lib.AD_get_locations(self.ad_obj, i)

    def get_rotations(self, i):
        return lib.AD_get_rotations(self.ad_obj, i)

    def get_control(self, n):
        return lib.CS_get_control(self.cs_obj, n)

    def compute_samples(self):
        lib.CS_compute_samples(self.cs_obj)

    def get_samples(self, n):
        return lib.CS_get_samples(self.cs_obj, n)

    return type("MrGeoLib", (), {\
        "__init__": init,\
        "torus_test": torus_test,\
        "test_bases": test_bases,\
        "fill": fill,\
        "get_num_objects": get_num_objects,\
        "get_num_frames": get_num_frames,\
        "get_scales": get_scales,\
        "get_locations": get_locations,\
        "get_rotations": get_rotations,\
        "get_control": get_control,\
        "compute_samples": compute_samples,\
        "get_samples": get_samples\
    })
